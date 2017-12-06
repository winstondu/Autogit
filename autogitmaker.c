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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/input.h>
#include <linux/kmod.h>
#include <linux/time.h>

#define AUTOGIT_VERSION "1.4"
// Will need to modify this
static char* remote = "https://winstondu:HASH@github.com/winstondu/RemoteTest.git"; /* where the remote goes to */


MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Winston Du");
MODULE_VERSION(AUTOGIT_VERSION);
MODULE_DESCRIPTION("Init a git folder and push it.");

module_param(remote, charp, 0644); /* allow remote to be a parameter */
MODULE_PARM_DESC(remote, "link to remote");

/* Kernel program calls*/
static int write( void )
{struct subprocess_info *sub_info;
char *argv[] = { "usr/bin/touch", "/home/winston/Documents/CS3281/test/michael.txt", NULL };
static char *envp[] = {
"HOME=/home/",
"TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
sub_info = call_usermodehelper_setup( argv[0], argv, envp, 0, NULL, NULL, NULL);
if (sub_info == NULL) return -ENOMEM;
return call_usermodehelper_exec( sub_info, UMH_WAIT_PROC );
}

static int git_init( void )
{
struct subprocess_info *sub_info;
/* must do via absolute path since exec calls dont have current dir */
char *argv[] = { "usr/bin/git", "-C", "/home/winston/Documents/CS3281/test/","init", NULL };
static char *envp[] = {
"HOME=/",
"TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL, NULL);
if (sub_info == NULL) return -ENOMEM;
return call_usermodehelper_exec( sub_info, UMH_WAIT_PROC );
}

static int git_add( void )
{
struct subprocess_info *sub_info;
char *argv[] = { "usr/bin/git", "-C", "/home/winston/Documents/CS3281/test/", "add", "-A", NULL };
static char *envp[] = {
"HOME=/",
"TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL, NULL);
if (sub_info == NULL) return -ENOMEM;
return call_usermodehelper_exec( sub_info, UMH_WAIT_PROC );
}

static int git_configure_name ( void )
{
struct subprocess_info *sub_info;
// Had to run this via sh because a regular invocation of git commit doesn't work for some reason.
char *argv[] = { "/bin/bash",  "-c", "/usr/bin/git -C /home/winston/Documents/CS3281/test/ config user.name john", NULL };
static char *envp[] = {
"HOME=/",
"TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL, NULL);
if (sub_info == NULL) return -ENOMEM;
return call_usermodehelper_exec( sub_info, 2);
}

static int git_configure_email ( void )
{
struct subprocess_info *sub_info;
// Had to run this via sh because a regular invocation of git commit doesn't work for some reason.
char *argv[] = { "/bin/bash",  "-c", "/usr/bin/git -C /home/winston/Documents/CS3281/test/ config user.email john@autogit.com", NULL };static char *envp[] = {
"HOME=/",
"TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL, NULL);
if (sub_info == NULL) return -ENOMEM;
return call_usermodehelper_exec( sub_info, 2);
}

static int git_commit( void )
{
struct subprocess_info *sub_info;
// Had to run this via sh because a regular invocation of git commit doesn't work for some reason.
// char *argv[] = { "/bin/sh", "/home/winston/Documents/CS3281/AutoGit/cool.sh", NULL };
char *argv[] = { "/bin/bash",  "-c", "/usr/bin/git -C /home/winston/Documents/CS3281/test/ commit -q --allow-empty-message --no-verify -m okc 2>/home/winston/Documents/CS3281/ok.txt 1>/home/winston/Documents/CS3281/ok2.txt", NULL };
// char *argv[] = { "usr/bin/git", "-C", "/home/winston/Documents/CS3281/test/", "commit", NULL };
static char *envp[] = {
"HOME=/",
"TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL, NULL);
if (sub_info == NULL) return -ENOMEM;
return call_usermodehelper_exec( sub_info, 2);
}

static int git_log( void )
{
struct subprocess_info *sub_info;
// Had to run this via sh because a regular invocation of git commit doesn't work for some reason.
char *argv[] = { "/bin/bash",  "-c","/bin/ls > /home/winston/Documents/CS3281/ok2.txt", NULL };
// char *argv[] = { "usr/bin/git", "-C", "/home/winston/Documents/CS3281/test/", "commit", "-m", "ok",NULL };
static char *envp[] = {
"HOME=/",
"TERM=linux",
"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL, NULL);
if (sub_info == NULL) return -ENOMEM;
return call_usermodehelper_exec( sub_info, UMH_WAIT_PROC );
}

static int git_push(void)
{
	struct subprocess_info *sub_info;
	char *argv[] = { "usr/bin/git", "-C", "/home/winston/Documents/CS3281/test/", "push", "placeholder", "-f", "--all", NULL };
	static char *envp[] = {
	"HOME=/",
	"TERM=linux",
	"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
	argv[4] = remote;
	sub_info = call_usermodehelper_setup( argv[0], argv, envp, GFP_ATOMIC, NULL, NULL, NULL);
	if (sub_info == NULL) return -ENOMEM;
	return call_usermodehelper_exec( sub_info, UMH_WAIT_PROC );
}

static int __init autogit_init(void)
{
	git_init();
	git_configure_name();
	git_configure_email();
	write();
	int t1 = git_add();
	printk("git add gave %d", t1);
	int t2= git_commit();
	printk("git commit gave %d", t2);
	int t3= git_log();
	printk("git log gave %d", t3);
	printk("\n");
	return 0;
}

static void __exit autogit_exit(void)
{
	git_push();
}

module_init(autogit_init);
module_exit(autogit_exit);
