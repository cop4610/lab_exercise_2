#include <linux/module.h>
#include <linux/sched.h>
#include <linux/fs/proc/array.c>
 struct task_struct *task, *t;
 

static int hello_init(void)
{
   for_each_process(task)
    {
	t = task;
	do {
      		 printk("Name: %s PID: [%d], parentPID: [%d] state: %s\n", t->comm, t->pid,t->parent->pid, get_task_state(t));
	}while_each_thread(task, t);
     } 
 
  return 0;
}

static void hello_exit(void)
{
 printk("Goodbye world\n");
 printk("Goodbye world 2\n");
 printk("Goodbye world 3\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("COP 4610");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World");
