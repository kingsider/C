/* 
 *  procfs.c -  ������ �������� ����� � /proc, ������� �������� ��� �� ������, ��� � �� ������.
 */
#include <linux/module.h>  /* ���������� ��� ������ ������ */
#include <linux/kernel.h>  /* ���-���� �� �������� � �����! */
#include <linux/proc_fs.h> /* ���������� ��� ������ � �������� �������� /proc */
#include <asm/uaccess.h>   /* ����������� ������� get_user � put_user */

/* 
 * ����� �������� ���������� ��������� ���������,
 * ������� ����� ���������� � ����, ����� ��������, ���
 * ������ ������������� ����� �������� ���� �� ������������
 */
#define MESSAGE_LENGTH 80
static char Message[MESSAGE_LENGTH];
static struct proc_dir_entry *Our_Proc_File;

#define PROC_ENTRY_FILENAME "rw_test"

static ssize_t module_output(struct file *filp, /* ��. include/linux/fs.h   */
             char *buffer,      /* ����� � ������� */
             size_t length,     /* ������ ������   */
             loff_t * offset)
{
  static int finished = 0;
  int i;
  char message[MESSAGE_LENGTH + 30];

  /* 
   * ��� ��������� �������� ����� ����� ������������ 0.
   * ���� ����� �� �������, ������� ����� ���������� 
   * �������� ������ �� �����,
   * ������ � ����������� ����. 
   */
  if (finished) {
    finished = 0;
    return 0;
  }
  /* 
   * ��� �������� ������ �� ������������ ���� � ������������ ������������ 
   * ������� ������������ put_user.
   * � �������� ����������� -- get_user.
   */
  sprintf(message, "Last input:%s", Message);
  for (i = 0; i < length && message[i]; i++)
    put_user(message[i], buffer + i);

  /* 
   * �������� ��������: � ������ �������� �� ������� �� �������������,
   * ��� ������ ��������� ������, ��� len, � ��������� ������ ��������� ���� ��������.
   * � �������� ��������, ���� ����� ��������� ������ ��� 
   * len, �� ������������ len, � ������� ��������� ������������ 
   * �� ����������� �������.
   */
  finished = 1;

  return i;             /* ������� ���������� "�����������" ���� */
}

static ssize_t
module_input(struct file *filp, const char *buff, size_t len, loff_t * off)
{
  int i;
  /* 
   * ����������� ������, ���������� �� ������������ � �����, 
   * ������� ������� ����� ������� �������� module_output.
   */
  for (i = 0; i < MESSAGE_LENGTH - 1 && i < len; i++)
    get_user(Message[i], buff + i);

  Message[i] = '\0';    /* ������� ������, ������������� �������� \0 */
  return i;
}

/* 
/* 
 * ��� ������� ��������� ������� � ����� �� ���������� �������� � ������
 * 0 -- ���������, �������� �������� -- ���������.
 *
 * �������� � ������ ����� ����:
 * 0 - ����������e (�� ����� ������ � ����� ��������)
 * 2 - ������ (�������� �� ������������ � ������ ����)
 * 4 - ������ (�������� �� ������ ���� � ������������)
 *
 * ��� ������� ��������� ����� ������� � �����
 * �����, ��������� �������� ls -l 
 * ����� ���� ��������������� �����.
 */

static int module_permission(struct inode *inode, int op, struct nameidata *foo)
{
  /* 
   * �������� ������ ������ ����, ��
   * ������ -- ������ root-� (uid 0)
   */
  if (op == 4 || (op == 2 && current->euid == 0))
    return 0;

  /* 
   * ���� ���-�� ���� -- ��������� ������
   */
        return -EACCES;
}

/* 
 * ���� ������ -- ���� ��� ��� ����� ������������ � ���-��
 * ������������, ��� ����� ������� -- ��� ��������� 
 * ������� ��������� � ������.
 */
int module_open(struct inode *inode, struct file *file)
{
  try_module_get(THIS_MODULE);
 return 0;
}

/* 
 * ���� ������ -- ��������� ������� ���������.
 */
int module_close(struct inode *inode, struct file *file)
{
  module_put(THIS_MODULE);
  return 0;             /* ��� ���������! */
}

static struct file_operations File_Ops_4_Our_Proc_File = {
        .read = module_output,
        .write = module_input,
        .open = module_open,
        .release = module_close,
};

/* 
 * �������� ��� ��������� ������� ������ �����. ����������
 * ��� ����, ����� ������� �������������� ���������
 * file_operations ������ �����, � ��� ��, ����� ������ �����
 * ������� ����������� ���� ������� � �����. ����� ����� ������� ������
 * ������ �������-������������, �� ��� ��� �� ����������.
 */

static struct inode_operations Inode_Ops_4_Our_Proc_File = {
  .permission = module_permission,      /* �������� ���� ������� */
};

/* 
 * ��������� � �������� ������� ������
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

