#include "ata.h"

void ata_busy_poll(void);
void ata_device_ready_poll(void);

void ata_busy_poll(void) {
  while (inb(STATUS_REGISTER) & BUSY_MASK);
  return;
}

void ata_drq_poll(void) {
  while ((inb(STATUS_REGISTER) & DATA_TRANSFER_REQUESTED_MASK) == 0);
  return;
}

void ata_device_ready_poll(void) {
  while ((inb(STATUS_REGISTER) & DEVICE_READY_MASK) == 0);
  return;
}

void ata_pio_read_loop(u16 *buf) {
    u16 data;
    for (s32 i = 0; i < 256; i++) {
      ata_busy_poll();
      ata_drq_poll();
      data = inw(DATA_REGISTER);
      buf[i] = data;

      u8 data_str[8];
      hex_to_ascii(data, data_str);
      kprint("Data_str: ");
      kprint(data_str);
    }

    for (s32 i = 0; i < 5; i++) inb(ALTERNATE_STATUS_REGISTER);
}

void ata_read_sector(u32 lba, u8 sectors, void* buf) {
  /*
    This bit is complicated.
    Send to the drive-head-register, 0xE0 for master, then set the last 4 bits
    of the lba. This register shares multiple values, so we do this here
  */
  outb(DRIVE_HEAD_REGISTER, 0xE0 | (0xF0 << 4) | ((lba >> 24) & 0x0F));
  // Send a null byte to error register as in specs
  outb(ERROR_REGISTER, 0x00);
  // Send sectors to Count Register. s=0==256 sectors! Can't read 0 at once
  outb(SECTOR_COUNT_REGISTER, sectors);
  // Eight low bits of LBA
  outb(LBA_LO_REGISTER, (u8) lba);
  // Next eight bits
  outb(LBA_MI_REGISTER, (u8) (lba >> 8));
  // Final eight bits
  outb(LBA_HI_REGISTER, (u8) (lba >> 16));
  // Send our read command
  outb(COMMAND_REGISTER, READ_SECTORS_WITH_RETRY);

  for (s32 i = 0; i < sectors; i++) {
    ata_pio_read_loop( (u16) (i * 512) + buf);
  }

  return;

}




// /*
//    BSY: a 1 means that the controller is busy executing a command. No register should be accessed (except the digital output register) while this bit is set.
//    RDY: a 1 means that the controller is ready to accept a command, and the drive is spinning at correct speed..
//    WFT: a 1 means that the controller detected a write fault.
//    SKC: a 1 means that the read/write head is in position (seek completed).
//    DRQ: a 1 means that the controller is expecting data (for a write) or is sending data (for a read). Don't access the data register while this bit is 0.
//    COR: a 1 indicates that the controller had to correct data, by using the ECC bytes (error correction code: extra bytes at the end of the sector that allows to verify its integrity and, sometimes, to correct errors).
//    IDX: a 1 indicates the the controller retected the index mark (which is not a hole on hard-drives).
//    ERR: a 1 indicates that an error occured. An error code has been placed in the error register.
//  */

// #define STATUS_BSY 0x80
// #define STATUS_RDY 0x40
// #define STATUS_DRQ 0x08
// #define STATUS_DF 0x20
// #define STATUS_ERR 0x01

// // #define STAT_ERR  (1 << 0) // Indicates an error occurred. Send a new command to clear it
// // #define STAT_DRQ  (1 << 3) // Set when the drive has PIO data to transfer, or is ready to accept PIO data.
// // #define STAT_SRV  (1 << 4) // Overlapped Mode Service Request.
// // #define STAT_DF   (1 << 5) // Drive Fault Error (does not set ERR).
// // #define STAT_RDY  (1 << 6) // Bit is clear when drive is spun down, or after an error. Set otherwise.
// // #define STAT_BSY  (1 << 7) // Indicates the drive is preparing to send/receive data (wait for it to clear). In case of 'hang' (it never clears), do a software reset.
// //

// //This is really specific to out OS now, assuming ATA bus 0 master
// //Source - OsDev wiki
// static void ATA_wait_BSY();
// static void ATA_wait_DRQ();
// void read_sectors_ATA_PIO(u32 target_address, u32 LBA, u8 sector_count)
// {

//         kprint("1\n");
//         ATA_wait_BSY();
//         kprint("2\n");
//         outb(0x1F6,0xE0 | ((LBA >>24) & 0xF));
//         kprint("3\n");
//         outb(0x1F2,sector_count);
//         kprint("4\n");
//         outb(0x1F3, (u8) LBA);
//         kprint("5\n");
//         outb(0x1F4, (u8)(LBA >> 8));
//         kprint("6\n");
//         outb(0x1F5, (u8)(LBA >> 16));
//         kprint("7\n");
//         outb(0x1F7,0x20); //Send the read command
//         kprint("8\n");

//         u16* target = (u16*) target_address;
//         kprint("9\n");

//         for (s32 j=0; j<sector_count; j++)
//         {
//                 kprint("10\n");
//                 ATA_wait_BSY();
//                 kprint("11\n");
//                 ATA_wait_DRQ();
//                 kprint("12\n");
//                 for(int i=0; i<256; i++) {
//                         kprint("13\n");
//                         target[i] = inw(0x1F0);
//                 }
//                 target+=256;
//         }
// }
// static void ATA_wait_BSY() //Wait for BSY to be 0.
// {
//         while(inb(0x1F7)&STATUS_BSY);
// }
// static void ATA_wait_DRQ() //Wait for DRQ to be 1.
// {
//         while(!(inb(0x1F7)&STATUS_RDY));
// }
// static void ATA_wait_RDY()  //Wait for RDY to be 1
// {
//         while(!(inb(0x1F7)&STATUS_RDY));
// }