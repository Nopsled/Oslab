#ifndef ATA_H
#define ATA_H

#include "../cpu/type.h"
#include "../cpu/timer.h"
#include "../drivers/screen.h"

void ata_send_command(u16 command);
void ata_read_sector(u32 lba, u8 sectors, void* buf);

// http://lateblt.tripod.com/atapi.htm
enum IDE_REGISTERS {
  DATA_REGISTER = 0x1F0,
  ERROR_REGISTER = 0x1F1,
  FEATURES_REGISTER = 0x1F1,
  SECTOR_COUNT_REGISTER = 0x1F2,
  LBA_LO_REGISTER = 0x1F3,
  LBA_MI_REGISTER = 0x1F4,
  LBA_HI_REGISTER = 0x1F5,
  DRIVE_HEAD_REGISTER = 0x1F6,
  STATUS_REGISTER = 0x1F7,
  COMMAND_REGISTER = 0x1F7,
  ALTERNATE_STATUS_REGISTER = 0x3F6,
  DEVICE_CONTROL_REGISTER = 0x3F6
};

enum STATUS_REGISTER_MASKS {
  ERROR_MASK = 1,
  INDEX_MARK_MASK = 2,
  DATA_CORRECTED_MASK = 4,
  DATA_TRANSFER_REQUESTED_MASK = 8,
  SEEK_COMPLETE_MASK = 16,
  DEVICE_FAULT_MASK = 32,
  DEVICE_READY_MASK = 64,
  BUSY_MASK = 128
};

enum ERROR_REGISTER_MASKS {
  ADDRESS_MARK_NOT_FOUND_MASK = 1,
  TRACK_ZERO_NOT_FOUND_MASK = 2,
  COMMAND_ABORTED_MASK = 4,
  MEDIA_CHANGE_REQUESTED = 8,
  ID_MARK_NOT_FOUND_MASK = 16,
  MEDIA_CHANGED_MASK = 32,
  UNCORRECTABLE_DATA_ERROR = 64,
  BAD_BLOCK_ERROR = 128
};

// https://wiki.osdev.org/ATA_Command_Matrix
enum ATA_COMMANDS {
  READ_SECTORS_WITH_RETRY = 0x20
};

#endif

// #ifndef ATA_H
// #define ATA_H

// #include "../cpu/type.h"
// #include "../cpu/timer.h"
// #include "../drivers/screen.h"


// void read_sectors_ATA_PIO(u32 target_address, u32 LBA, u8 sector_count);

// #endif

// #define ATA_PRIMARY_DATA         0x1F0
// #define ATA_PRIMARY_ERR          0x1F1
// #define ATA_PRIMARY_SECCOUNT     0x1F2
// #define ATA_PRIMARY_LBA_LO       0x1F3
// #define ATA_PRIMARY_LBA_MID      0x1F4
// #define ATA_PRIMARY_LBA_HI       0x1F5
// #define ATA_PRIMARY_DRIVE_HEAD   0x1F6
// #define ATA_PRIMARY_COMM_REGSTAT 0x1F7
//
// #define ATA_DATA  0 /* data register */
// #define ATA_ERROR 1 /* error register */
// #define ATA_COUNT 2 /* sectors to transfer */
// #define ATA_SECTOR  3 /* sector number */
// #define ATA_CYL_LO  4 /* low byte of cylinder number */
// #define ATA_CYL_HI  5 /* high byte of cylinder number */
// #define ATA_FDH   6 /* flags, drive and head */
// #define ATA_STATUS  7
// #define ATA_COMMAND 7
// #define ATA_CONTROL 0x206
//
// #define ATA_COMMAND_IDENTIFY    0xEC
// #define ATA_PRIMARY_ALTSTAT_DCR  0x3F6
//
// #define STAT_ERR  (1 << 0) // Indicates an error occurred. Send a new command to clear it
// #define STAT_DRQ  (1 << 3) // Set when the drive has PIO data to transfer, or is ready to accept PIO data.
// #define STAT_SRV  (1 << 4) // Overlapped Mode Service Request.
// #define STAT_DF   (1 << 5) // Drive Fault Error (does not set ERR).
// #define STAT_RDY  (1 << 6) // Bit is clear when drive is spun down, or after an error. Set otherwise.
// #define STAT_BSY  (1 << 7) // Indicates the drive is preparing to send/receive data (wait for it to clear). In case of 'hang' (it never clears), do a software reset.
//
// u8 identify();
