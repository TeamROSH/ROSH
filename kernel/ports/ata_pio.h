#ifndef ATA_PIO_H
#define ATA_PIO_H

void read_sectors(uint32_t target_address, uint32_t LBA, uint8_t sector_count);
void write_sectors(uint32_t target_address, uint32_t LBA, uint8_t sector_count);

#endif