#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/device.h>
#define STRING_SIZE 101
MODULE_LICENSE("Dual BSD/GPL");

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;

char string[STRING_SIZE];
int endRead = 0;

int stred_open(struct inode *pinode, struct file *pfile);
int stred_close(struct inode *pinode, struct file *pfile);
ssize_t stred_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t stred_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = stred_open,
	.read = stred_read,
	.write = stred_write,
	.release = stred_close,
};


int stred_open(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully opened stred\n");
		return 0;
}

int stred_close(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully closed stred\n");
		return 0;
}

ssize_t stred_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) 
{
	int ret;
	char buff[STRING_SIZE];
	long int len = 0;
	if (endRead){
		endRead = 0;
		return 0;
	}

	
	len = scnprintf(buff, STRING_SIZE, "%s ", string);
	if(len == 0)
	{
		printk(KERN_WARNING "String je prazan\n");
	}
	else
	{
		ret = copy_to_user(buffer, buff, len);
		printk(KERN_INFO "String je:%s\n", string);
		
		if(ret)
			return -EFAULT;
		printk(KERN_INFO "Succesfully read\n");
		endRead = 1;
	}

	return len;
}

ssize_t stred_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
	char buff[STRING_SIZE];	  
	char operation[STRING_SIZE];    //izdvaja unetu opciju upisa ili modifikacije stringa
	char rightSide[STRING_SIZE+1];  //izdvaja ostatak komande ukoliko ona sadrzi '=' 
	int leng = 0;			//pomocna promenljiva za duzinu stringa
	int i = 0;
	int ret;

	ret = copy_from_user(buff, buffer, length);
	if(ret)
		return -EFAULT;
	buff[length-1] = '\0';
	

	while(buff[i]!= '=' && buff[i]!= '\0')
	{
		operation[i] = buff[i];             //kopiranje operacije iz buff
		i++;
	}
	operation[i]='\0';
	if(buff[i]=='\0')
	{
		strcpy(rightSide, &buff[i]);	    //rightSide = '\0' ako nema '='  
	}
	else
	{
		strcpy(rightSide, &buff[++i]);      //rightSide je desno od '='
	}
	
	ret = sscanf(buff, "%s", buff);

	if(ret==1)//one parameter parsed in sscanf
	{
		printk(KERN_INFO "Komanda: %s, nastavak:%s\n", operation, rightSide); 
		
		if(!strcmp(operation, "string"))
		{
			if(strlen(rightSide) > STRING_SIZE - 1)
			{
				printk(KERN_WARNING "Unet je predugacak string\n");
			}
			else
			{
				strcpy(string, rightSide);
				printk(KERN_INFO "Uspesno unet string\n");
			}
		}
		else if(!strcmp(operation, "clear"))
		{
			for(i = 0; i < STRING_SIZE; i++)
			{
				string[i] = 0;	
			}
			printk(KERN_INFO "Uspesno obrisan string\n");
		}
		else if(!strcmp(operation, "append"))
		{
			if(strlen(rightSide) + strlen(rightSide) > STRING_SIZE - 1)
			{
				printk(KERN_WARNING "Unet je predugacak string\n");
			}
			else
			{
				strcat(string, rightSide);
				printk(KERN_INFO "Uspesno dodat string\n");
			}
		}
		else if(!strcmp(operation, "shrink"))
		{
			//strim(string);    <- ne radi
			
			leng = strlen(string);
			while(string[0] == ' ')
			{
				for(i = 0; i < strlen(string); i++)
				{
					string[i] = string[i+1];
				}
				leng--;
				string[leng] = '\0';
			}		
			while(string[leng-1] == ' ')
			{
				string[leng-1] = '\0';
				leng--;
			}
			string[leng] = '\0';
			
			printk(KERN_INFO "Uspesno uklonjeni razmaci sa krajeva\n");
		}
		else if(!strcmp(operation, "truncate"))
		{
			int br;
			kstrtoint(rightSide, 10, &br);
			if(strlen(string) < br)
			{
				printk(KERN_WARNING "String je prekratak za dati broj\n");
			}
			else
			{
				leng = strlen(string);
				for(i = 0; i < br; i++)
				{
					string[leng-1] = '\0';
					leng--;
				}
				printk(KERN_INFO "Uspesno izbrisano poslednjih %d mesta", br);
			}
		}
		else if(!strcmp(operation, "remove"))
		{
			if(strlen(string) < strlen(rightSide))
			{
				printk(KERN_WARNING "String ne sadrzi uneti podstring\n");
			}
			else
			{
				char *seq = strstr(string, rightSide);
				int rightSide_len = strlen(rightSide);
				
				leng = strlen(string);
			//	leng = strlen(string) - rightSide_len;

				string[leng - strlen(seq)] = '\0';

				for(i = 0; i < rightSide_len; i++)
				{
					seq++;
				}
				strcat(string, seq);

				printk(KERN_INFO "Uspesno izbrisan podstring:%s", rightSide);
			}
		}
		else
		{
			printk(KERN_WARNING "Neispravna operacija: %s", operation);
		}
	}
	else
	{
		printk(KERN_WARNING "Wrong command format\n");
	}
		 
	

	return length;
}

static int __init stred_init(void)
{
   int ret = 0;
	int i=0;

	//Initialize array
//	for (i=0; i<STRING_SIZE; i++)
		string[i] = '\0';

   ret = alloc_chrdev_region(&my_dev_id, 0, 1, "stred");
   if (ret){
      printk(KERN_ERR "failed to register char device\n");
      return ret;
   }
   printk(KERN_INFO "char device region allocated\n");

   my_class = class_create(THIS_MODULE, "stred_class");
   if (my_class == NULL){
      printk(KERN_ERR "failed to create class\n");
      goto fail_0;
   }
   printk(KERN_INFO "class created\n");
   
   my_device = device_create(my_class, NULL, my_dev_id, NULL, "stred");
   if (my_device == NULL){
      printk(KERN_ERR "failed to create device\n");
      goto fail_1;
   }
   printk(KERN_INFO "device created\n");

	my_cdev = cdev_alloc();	
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret)
	{
      printk(KERN_ERR "failed to add cdev\n");
		goto fail_2;
	}
   printk(KERN_INFO "cdev added\n");
   printk(KERN_INFO "Hello world\n");

   return 0;

   fail_2:
      device_destroy(my_class, my_dev_id);
   fail_1:
      class_destroy(my_class);
   fail_0:
      unregister_chrdev_region(my_dev_id, 1);
   return -1;
}

static void __exit stred_exit(void)
{
   cdev_del(my_cdev);
   device_destroy(my_class, my_dev_id);
   class_destroy(my_class);
   unregister_chrdev_region(my_dev_id,1);
   printk(KERN_INFO "Goodbye, cruel world\n");
}


module_init(stred_init);
module_exit(stred_exit);

