/*
 * Linux Virtual Wireless Network Implementation
 * Copyright 2012, Bruno Ferreira <bruno@info.ufrn.br>
 * Copyright 2014-2017, Leonardo Dantas <lodantas@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

//#ifdef MAC80211_MAIN_C
/**
 * code used for mac80211 main.c file and mac80211_hwsim.c
 * mac80211 tx.c file are using as externs funcions. 
 **/
#ifndef MAC80211_TX_C

spinlock_t lvwnet_lock;
//spinlock_t lvwnet_tx_lock;
//EXPORT_SYMBOL_GPL(lvwnet_tx_lock);
unsigned short int lvwnet_status;

//void (*lvwnet_ptr_skb)(struct sk_buff *skb);
//EXPORT_SYMBOL_GPL(lvwnet_ptr_skb);
//void (*lvwnet_ptr_hw)(struct ieee80211_hw *hw);
//EXPORT_SYMBOL_GPL(lvwnet_ptr_hw);
extern void (*lvwnet_ptr_hw)(struct ieee80211_hw *hw);
extern void (*lvwnet_ptr_skb)(struct sk_buff *skb);
extern void lvwnet_set_loaded(void);
extern void lvwnet_set_unloaded(void);
extern spinlock_t lvwnet_tx_lock;
#ifdef HWSIM_C
extern spinlock_t lvwgw_lock;
extern void lvwnet_send_skb_from_mac80211(struct sk_buff *skb);
extern void lvwnet_send_hw_from_mac80211(struct ieee80211_hw *hw);
extern int lvwnet_is_loaded(void);
#endif

/* init... */
void lvwnet_init(void)
{
    spin_lock_init(&lvwnet_lock);
    spin_lock_init(&lvwnet_tx_lock);
    lvwnet_status = 0;
    lvwnet_ptr_hw = NULL;
    lvwnet_ptr_skb = NULL;
    printk(KERN_INFO "lvwnet: modified mac80211 started...\n");
}

void lvwnet_exit(void)
{
    spin_lock_init(&lvwnet_lock);
    spin_lock_init(&lvwnet_tx_lock);
    lvwnet_status  = 0;
    lvwnet_ptr_hw  = NULL;
    lvwnet_ptr_skb = NULL;
    printk(KERN_INFO "lvwnet: exiting from modified mac80211...\n");
}

#ifdef MAC80211_TX_C
/* verify if lvwnet is already loaded.
 * lock safe with spin_lock.
 */
int lvwnet_is_loaded(void)
{
    int _return = 0;
    spin_lock(&lvwnet_lock);

    if (lvwnet_status)
         _return = 1;

    spin_unlock(&lvwnet_lock);

    return _return;
}
EXPORT_SYMBOL_GPL(lvwnet_is_loaded);
#endif

/* set lvwnet as loaded */
/*void lvwnet_set_loaded(void)
{
    spin_lock(&lvwnet_lock);
    lvwnet_status = 1;
    spin_unlock(&lvwnet_lock);
    printk(KERN_INFO "lvwnet: lvwnet is now set as loaded. \n ");

}
EXPORT_SYMBOL_GPL(lvwnet_set_loaded);
*/
/* set lvwnet as unloaded */
/*void lvwnet_set_unloaded(void)
{
    spin_lock(&lvwnet_lock);
    lvwnet_status = 0;
    spin_unlock(&lvwnet_lock);
    printk(KERN_INFO "lvwnet: lvwnet is now set as unloaded. \n ");
}
EXPORT_SYMBOL_GPL(lvwnet_set_unloaded);
*/
void lvwnet_send_hw(struct ieee80211_hw *hw)
{
    if (lvwnet_is_loaded()) {
        if (lvwnet_ptr_hw == NULL) {
            printk(KERN_INFO "lvwnet: lvwnet_ptr_hw is NULL! [%s].\n", __func__);
        } else {
            //printk(KERN_DEBUG "lvwnet: lvwnet_ptr_hw is set. Sending hw. [%s].\n", __func__);
            (lvwnet_ptr_hw)(hw);
        }
    } else {
        printk(KERN_INFO "lvwnet: lvwnet is unloaded. Nothing to do [%s].\n", __func__);
    }
}
EXPORT_SYMBOL_GPL(lvwnet_send_hw);

void lvwnet_send_skb(struct sk_buff *skb)
{
    spin_lock(&lvwnet_tx_lock);
    if (lvwnet_is_loaded()) {
        if (lvwnet_ptr_skb == NULL) {
            printk(KERN_INFO "lvwnet: lvwnet_ptr_skb is NULL! [%s].\n", __func__);
        } else {
            (lvwnet_ptr_skb)(skb);
        }
    } else {
        printk(KERN_INFO "lvwnet: lvwnet is unloaded. Nothing to do [%s].\n", __func__);
    }
    spin_unlock(&lvwnet_tx_lock);
}
/* is exported to be used from tx.c */
EXPORT_SYMBOL_GPL(lvwnet_send_skb);
#endif


#ifdef MAC80211_TX_C
//#ifndef MAC80211_MAIN_C
extern spinlock_t lvwgw_lock;
extern void lvwnet_send_skb_from_mac80211(struct sk_buff *skb);
extern void lvwnet_send_hw_from_mac80211(struct ieee80211_hw *hw);
#endif

