#ifndef DEVICES_H
#define DEVICES_H

// Master header file for all devices
#include "io.h"
#include "pc.h"
#include "util.h"

void dma_init(void);
void cmos_init(uint64_t now);
void pit_init(void);
void pic_init(struct pc_settings* pc);
void kbd_init(void);
void vga_init(int);
void ide_init(struct pc_settings* pc);
void fdc_init(struct pc_settings* pc);

// XXX:
#define floppy_get_type(id) 0

void cmos_set(uint8_t where, uint8_t data);
uint8_t cmos_get(uint8_t where);
void pic_raise_irq(int);
void pic_lower_irq(int);
uint8_t pic_get_interrupt(void);
int pic_has_interrupt(void);
void kbd_add_key(uint8_t data);
void kbd_mouse_down(int left, int center, int right);
void kbd_send_mouse_move(int xrel, int yrel);

void vga_update(void);
void vga_restore_from_ptr(void* ptr);
void* vga_get_ptr(void);

// Timer functions, in lieu of "real" timer functions
int cmos_clock(itick_t now);
int pit_timer(itick_t now);

int cmos_next(itick_t now);
int pit_next(itick_t now);
int apic_next(itick_t now);
int floppy_next(itick_t now);

// The mouse button has just suddenly been pressed
#define MOUSE_STATUS_PRESSED 1
// The mouse button has been released
#define MOUSE_STATUS_RELEASED 0
// The mouse button state has not changed
#define MOUSE_STATUS_NOCHANGE 2

// DMA support
void* fdc_get_dma_buf(void);
void fdc_handle_transfer_end(void);
void dma_raise_dreq(int line);

// PCI support

// Callback for when a byte in PCI memory is modified. "addr" is the offset, and "ptr" points to the base of the 256-byte block
typedef int (*pci_conf_write_cb)(uint8_t* ptr, uint8_t addr, uint8_t data);
void pci_init(struct pc_settings* pc);
void pci_init_mem(void*);
void* pci_create_device(uint32_t bus, uint32_t device, uint32_t function, pci_conf_write_cb cb);

// PCI IDE support
void ide_write_prdt(uint32_t addr, uint32_t data);
uint32_t ide_read_prdt(uint32_t addr);

// (I/O)APIC support
void ioapic_init(struct pc_settings* pc);
void apic_init(struct pc_settings* pc);
int apic_is_enabled(void);

// Handled by the PIC
void ioapic_lower_irq(int);
void ioapic_raise_irq(int);
void ioapic_remote_eoi(int irq);
int apic_has_interrupt(void);
int apic_get_interrupt(void);
void apic_receive_bus_message(int vector, int type, int trigger_mode);

#endif