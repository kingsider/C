/* 
 *  procfs.c -  Пример создания файла в /proc, который доступен как на чтение, так и на запись.
 */
#include <linux/module.h>  /* Необходимо для любого модуля */
#include <linux/kernel.h>  /* Все-таки мы работаем с ядром! */
#include <linux/proc_fs.h> /* Необходимо для работы с файловой системой /proc */
#include <asm/uaccess.h>   /* определения функций get_user и put_user */

/* 
 * Место хранения последнего принятого сообщения,
 * которое будет выводиться в файл, чтобы показать, что
 * модуль действительно может получать ввод от пользователя
 */
#define MESSAGE_LENGTH 80
static char Message[MESSAGE_LENGTH];
static struct proc_dir_entry *Our_Proc_File;

#define PROC_ENTRY_FILENAME "rw_test"

static ssize_t module_output(struct file *filp, /* см. include/linux/fs.h   */
             char *buffer,      /* буфер с данными */
             size_t length,     /* размер буфера   */
             loff_t * offset)
{
  static int finished = 0;
  int i;
  char message[MESSAGE_LENGTH + 30];

  /* 
   * Для индикации признака конца файла возвращается 0.
   * Если этого не сделать, процесс будет продолжать 
   * пытаться читать из файла,
   * угодив в бесконечный цикл. 
   */
  if (finished) {
    finished = 0;
    return 0;
  }
  /* 
   * Для передачи данных из пространства ядра в пространство пользователя 
   * следует использовать put_user.
   * В обратном направлении -- get_user.
   */
  sprintf(message, "Last input:%s", Message);
  for (i = 0; i < length && message[i]; i++)
    put_user(message[i], buffer + i);

  /* 
   * Обратите внимание: в данной ситуации мы исходим из предположения,
   * что размер сообщения меньше, чем len, в противном случае сообщение будт обрезано.
   * В реальной ситуации, если длина сообщения больше чем 
   * len, то возвращается len, а остаток сообщения возвращается 
   * на последующих вызовах.
   */
  finished = 1;

  return i;             /* Вернуть количество "прочитанных" байт */
}

static ssize_t
module_input(struct file *filp, const char *buff, size_t len, loff_t * off)
{
  int i;
  /* 
   * Переместить данные, полученные от пользователя в буфер, 
   * который позднее будет выведен функцией module_output.
   */
  for (i = 0; i < MESSAGE_LENGTH - 1 && i < len; i++)
    get_user(Message[i], buff + i);

  Message[i] = '\0';    /* Обычная строка, завершающаяся символом \0 */
  return i;
}

/* 
/* 
 * Эта функция принимает решение о праве на выполнение операций с файлом
 * 0 -- разрешено, ненулеое значение -- запрещено.
 *
 * Операции с файлом могут быть:
 * 0 - Исполнениеe (не имеет смысла в нашей ситуации)
 * 2 - Запись (передача от пользователя к модулю ядра)
 * 4 - Чтение (передача от модуля ядра к пользователю)
 *
 * Эта функция проверяет права доступа к файлу
 * Права, выводимые командой ls -l 
 * могут быть проигнорированы здесь.
 */

static int module_permission(struct inode *inode, int op, struct nameidata *foo)
{
  /* 
   * Позволим любому читать файл, но
   * писать -- только root-у (uid 0)
   */
  if (op == 4 || (op == 2 && current->euid == 0))
    return 0;

  /* 
   * Если что-то иное -- запретить доступ
   */
        return -EACCES;
}

/* 
 * Файл открыт -- пока нам нет нужды беспокоиться о чем-то
 * единственное, что нужно сделать -- это нарастить 
 * счетчик обращений к модулю.
 */
int module_open(struct inode *inode, struct file *file)
{
  try_module_get(THIS_MODULE);
 return 0;
}

/* 
 * Файл закрыт -- уменьшить счетчик обращений.
 */
int module_close(struct inode *inode, struct file *file)
{
  module_put(THIS_MODULE);
  return 0;             /* все нормально! */
}

static struct file_operations File_Ops_4_Our_Proc_File = {
        .read = module_output,
        .write = module_input,
        .open = module_open,
        .release = module_close,
};

/* 
 * Операции над индексной записью нашего файла. Необходима
 * для того, чтобы указать местоположение структуры
 * file_operations нашего файла, а так же, чтобы задать адрес
 * функции определения прав доступа к файлу. Здесь можно указать адреса
 * других функций-обработчиков, но нас они не интересуют.
 */

static struct inode_operations Inode_Ops_4_Our_Proc_File = {
  .permission = module_permission,      /* проверка прав доступа */
};

/* 
 * Начальная и конечная функции модуля
 */
int init_module()
{
  int rv = 0;
 Our_Proc_File = create_proc_entry(PROC_ENTRY_FILENAME, 0644, NULL);
  Our_Proc_File->owner = THIS_MODULE;
  Our_Proc_File->proc_iops = &Inode_Ops_4_Our_Proc_File;
  Our_Proc_File->proc_fops = &File_Ops_4_Our_Proc_File;
  Our_Proc_File->mode = S_IFREG | S_IRUGO | S_IWUSR;
  Our_Proc_File->uid = 0;
  Our_Proc_File->gid = 0;
  Our_Proc_File->size = 80;

  if (Our_Proc_File == NULL) {
    rv = -ENOMEM;
    remove_proc_entry(PROC_ENTRY_FILENAME, &proc_root);
    printk(KERN_INFO "Error: Could not initialize /proc/test\n");
  }

  return rv;
}

void cleanup_module()
{
  remove_proc_entry(PROC_ENTRY_FILENAME, &proc_root);
}

