/*
 * A Linux kernel module that creates a test folder.
 *
 * Author: Winston Du
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * See <http://www.gnu.org/licenses/>.
 */

#include <linux/init.h>
#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

#define AUTOGIT_VERSION "1.4"
// Parameters (remote and local are required).
static char *remote =
    "https://<githubusername>:<HASHtoken>@github.com/<githubusername>/"
    "<repo>.git";              /* where the remote goes to */
static char *local = "/home/"; /* where you want to put the local file */
static char *name = "johndoe";
static char *email = "johndoe@anonymous.com";
// General Buffer
static char buffer[200] = {0};
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Winston Du");
MODULE_VERSION(AUTOGIT_VERSION);
MODULE_DESCRIPTION("Init a git folder and push it.");

module_param(remote, charp, 0644); /* allow remote to be a parameter */
module_param(local, charp, 0644);  /* allow local to be a parameter */
module_param(name, charp, 0644);   /* allow name to be a parameter */
module_param(email, charp, 0644);  /* allow remote to be a parameter */
MODULE_PARM_DESC(remote, "link to remote");
MODULE_PARM_DESC(local, "path of write folder");
MODULE_PARM_DESC(name, "your name (optional)");
MODULE_PARM_DESC(email, "your email (optional)");

/* Kernel program calls*/

/* Dummy test call that writes*/
static int write(void) {
  struct subprocess_info *sub_info;
  char *argv[] = {"/bin/bash", "-c", "<write command>", NULL};
  static char *envp[] = {"HOME=/home/", "TERM=linux",
                         "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
  char *template = "/bin/echo pikachu >> ";
  char *filename = "/michael.txt";
  /* modify argv with variables */
  buffer[0] = '\0';
  strcat(buffer, template);
  strcat(buffer, local);
  strcat(buffer, filename);
  argv[2] = buffer;
  /* Execute process */
  sub_info =
      call_usermodehelper_setup(argv[0], argv, envp, 0, NULL, NULL, NULL);
  if (sub_info == NULL) return -ENOMEM;
  return call_usermodehelper_exec(sub_info, UMH_WAIT_PROC);
}

struct file *file_open(const char *path, int flags, int rights) 
{
    struct file *filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if (IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}

static int write2(void) {
char* filename = "/home/winston/Documents/CS3281/no";
struct file *file = file_open(filename, O_CREAT | O_RDWR, 0644);
void* data = "write test test test test";  //Needs to be a kernel pointer, not userspace pointer
int pos = 0;
file_write(file, pos, data, 20);
filp_close(file,NULL);
return 0;
}

static int git_init(void) {
  struct subprocess_info *sub_info;
  /* must do via absolute path since exec calls dont have current dir */
  char *argv[] = {"usr/bin/git", "-C", "/specified/local/path", "init", NULL};
  static char *envp[] = {"HOME=/", "TERM=linux",
                         "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
  /* modify argv with variables */
  argv[2] = local;
  /* Execute process */
  sub_info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC, NULL,
                                       NULL, NULL);
  if (sub_info == NULL) return -ENOMEM;
  return call_usermodehelper_exec(sub_info, UMH_WAIT_PROC);
}

static int git_add(void) {
  struct subprocess_info *sub_info;
  char *argv[] = {"usr/bin/git", "-C", "/specified/local/path",
                  "add",         "-A", NULL};
  static char *envp[] = {"HOME=/", "TERM=linux",
                         "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
  /* modify argv with variables */
  argv[2] = local;
  /* Execute process */
  sub_info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC, NULL,
                                       NULL, NULL);
  if (sub_info == NULL) return -ENOMEM;
  return call_usermodehelper_exec(sub_info, UMH_WAIT_PROC);
}

static int git_configure_name(void) {
  struct subprocess_info *sub_info;
  char *argv[] = {"usr/bin/git", "-C",        "/specified/local/path",
                  "config",      "user.name", "<name>",
                  NULL};
  static char *envp[] = {"HOME=/", "TERM=linux",
                         "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
  /* modify argv with variables */
  argv[2] = local;
  argv[5] = name;
  /* Execute process */
  sub_info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC, NULL,
                                       NULL, NULL);
  if (sub_info == NULL) return -ENOMEM;
  return call_usermodehelper_exec(sub_info, 2);
}

static int git_configure_email(void) {
  struct subprocess_info *sub_info;
  char *argv[] = {"usr/bin/git", "-C",         "/specified/local/path",
                  "config",      "user.email", "<email>",
                  NULL};
  static char *envp[] = {"HOME=/", "TERM=linux",
                         "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
  /* modify argv with variables */
  argv[2] = local;
  argv[5] = email;
  /* Execute process */
  sub_info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC, NULL,
                                       NULL, NULL);
  if (sub_info == NULL) return -ENOMEM;
  return call_usermodehelper_exec(sub_info, 2);
}

static int git_commit(void) {
  struct subprocess_info *sub_info;
  char *argv[] = {"usr/bin/git", "-C", "/specified/local/path",
                  "commit",      "-q", "-m",
                  "msg",         NULL};
  static char *envp[] = {"HOME=/", "TERM=linux",
                         "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
  /* modify argv with variables */
  char numbuf[256];
  numbuf[0] = '\0';  // Ensure null-terminated.
  argv[2] = local;
  int i = 50;
  sprintf(numbuf, "%d", i);
  argv[6] = numbuf;
  /* Execute process */
  sub_info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC, NULL,
                                       NULL, NULL);
  if (sub_info == NULL) return -ENOMEM;
  return call_usermodehelper_exec(sub_info, 2);
}

static int git_push(void) {
  struct subprocess_info *sub_info;
  char *argv[] = {"usr/bin/git",
                  "-C",
                  "/specified/local/path",
                  "push",
                  "<remote repo>",
                  "-f",
                  "--all",
                  NULL};
  static char *envp[] = {"HOME=/", "TERM=linux",
                         "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};
  /* modify argv with variables */
  argv[2] = local;
  argv[4] = remote;
  /* Execute process */
  sub_info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC, NULL,
                                       NULL, NULL);
  if (sub_info == NULL) return -ENOMEM;
  return call_usermodehelper_exec(sub_info, UMH_WAIT_PROC);
}

static int __init autogit_init(void) {
  int t1, t2, t3, t4, t5, t6;
  t1 = git_init();
  printk("git init gave %d", t1);
  if (t1 != 0) {
    printk("git init failed. Are you sure the repo you specified is valid?");
    return 128;
  }
  t2 = git_configure_name();
  printk("git configure name gave %d", t2);
  t3 = git_configure_email();
  printk("git configure email gave %d", t3);
  t4 = write();
  write2();
  printk("write gave %d", t4);
  t5 = git_add();
  printk("git add gave %d", t5);
  t6 = git_commit();
  printk("git commit gave %d", t6);
  printk("\n");
  return 0;
}

static void __exit autogit_exit(void) {
  int t7 = git_push();
  printk("git push gave %d", t7);
  if (t7 != 0) {
    printk("Error: make sure you have a pushable remote!");
  }
}

module_init(autogit_init);
module_exit(autogit_exit);
