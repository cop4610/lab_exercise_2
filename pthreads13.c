#include <linux/module.h>
#include <linux/sched.h>
#include <linux/string.h>
struct task_struct *task, *t;


static int hello_init(void)
{
    for_each_process(task){
	       t = task;
           if(current->pid == t->pid){
               printk("Printing all current process threads");
               do {
                   long l = t->state;
                   char s[20];
                   if(l==-1){
                       strcpy(s, "UNRUNNABLE");
                   }else if (l==0){
                       strcpy(s, "RUNNING");
                   }else{
                       strcpy(s, "STOPPED");
                   }
                   printk("\tName: %s\t PID: [%d]\t parentPID: [%d]\t state: %s\n", t->comm, t->pid,t->parent->pid, s);
               }while_each_thread(task, t);
           }else {
               long l = t->state;
               char s[20];
               if(l==-1){
                   strcpy(s, "UNRUNNABLE");
               }else if (l==0){
                   strcpy(s, "RUNNING");
               }else{
                   strcpy(s, "STOPPED");
               }
               printk("Name: %s\t PID: [%d]\t parentPID: [%d]\t state: %s\n", t->comm, t->pid,t->parent->pid, s);
           }

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
