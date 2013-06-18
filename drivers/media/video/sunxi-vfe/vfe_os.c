#include <linux/module.h>
#include "vfe_os.h"

//for internel driver debug
#define DBG_EN      0   
//debug level 0~3
#define DBG_LEVEL   3

//for internel driver debug
#if(DBG_EN==1)    
#define vfe_dbg(l,x,arg...) if(l <= DBG_LEVEL) printk(KERN_DEBUG"[VFE_DEBUG]"x,##arg)
#else
#define vfe_dbg(l,x,arg...) 
#endif

#define vfe_err(x,arg...) printk(KERN_ERR"[VFE_ERR]"x,##arg)
#define vfe_warn(x,arg...) printk(KERN_WARNING"[VFE_WARN]"x,##arg)
#define vfe_print(x,arg...) printk(KERN_NOTICE"[VFE]"x,##arg)

extern int gpio_request(unsigned gpio, const char *label);
extern int gpio_free(unsigned gpio);
extern int gpio_direction_output(unsigned gpio, int value);
extern int gpio_direction_input(unsigned gpio);

struct clk *os_clk_get(struct device *dev, const char *id)  
{
  return clk_get(dev, id);
}
EXPORT_SYMBOL_GPL(os_clk_get);

void  os_clk_put(struct clk *clk)
{
  clk_put(clk);
}
EXPORT_SYMBOL_GPL(os_clk_put);

int os_clk_set_parent(struct clk *clk, struct clk *parent) 
{
  return clk_set_parent(clk, parent);
}
EXPORT_SYMBOL_GPL(os_clk_set_parent);

int os_clk_set_rate(struct clk *clk, unsigned long rate) 
{
  return clk_set_rate(clk, rate);
}
EXPORT_SYMBOL_GPL(os_clk_set_rate);

int os_clk_enable(struct clk *clk) 
{
  return clk_enable(clk);
}
EXPORT_SYMBOL_GPL(os_clk_enable);

void os_clk_disable(struct clk *clk) 
{
   clk_disable(clk);
}
EXPORT_SYMBOL_GPL(os_clk_disable);

int os_clk_reset_assert(struct clk *clk) 
{
  return clk_reset(clk,AW_CCU_CLK_RESET);
}
EXPORT_SYMBOL_GPL(os_clk_reset_assert);

int os_clk_reset_deassert(struct clk *clk) 
{
  return clk_reset(clk,AW_CCU_CLK_NRESET);
}
EXPORT_SYMBOL_GPL(os_clk_reset_deassert);

int os_request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
      const char *name, void *dev)
{
  return request_irq(irq,handler,flags,name,dev);
}
EXPORT_SYMBOL_GPL(os_request_irq);

__hdle os_gpio_request(unsigned gpio, const char *label)
{
  return gpio_request(gpio, label);
}
EXPORT_SYMBOL_GPL(os_gpio_request);

int os_gpio_release(unsigned gpio)
{
  return gpio_free(gpio);
}
EXPORT_SYMBOL_GPL(os_gpio_release);

int os_gpio_write(struct gpio_config *gpio, unsigned int status)
{
	if(gpio->gpio==GPIO_INDEX_INVALID)
	{
		vfe_err("invalid gpio\n");
		return -1;
	}
	  
	if(gpio->mul_sel==1)
	{
	  gpio_direction_output(gpio->gpio, status);
	  gpio->data=status;
	} else {
	  vfe_err("gpio is not in output function\n");
	}

  return 0;
}
EXPORT_SYMBOL_GPL(os_gpio_write);

int os_gpio_set_status(struct gpio_config *gpio, unsigned int status)
{
	if(1 == status && gpio->gpio!=0) {	/* output */
		if(0 != gpio_direction_output(gpio->gpio, gpio->data))
			vfe_err("gpio_direction_output failed\n");
		else {
		  vfe_dbg(0,"gpio_direction_output gpio[%d]=%d\n",gpio->gpio, gpio->data);
			gpio->mul_sel=status;
		}
	} else if(0 == status && gpio->gpio!=0) {  /* input */
	  if(0 != gpio_direction_input(gpio->gpio) )
		vfe_err("gpio_direction_input failed\n");
	  else
		gpio->mul_sel=status;
	}

  return 0;
}
EXPORT_SYMBOL_GPL(os_gpio_set_status);

MODULE_AUTHOR("raymonxiu");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Video front end OSAL for sunxi");