/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
//      read data from IDE disk. First issue a read request through
//      disk register and then copy data from disk buffer
//      (512 bytes, a sector) to destination array.
//
// Parameters:
//      diskno: disk number.
//      secno: start sector number.
//      dst: destination for data read from IDE disk.
//      nsecs: the number of sectors to read.
//
// Post-Condition:
//      If error occurred during read the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
        // 0x200: the size of a sector: 512 bytes.
        int offset_begin = secno * 0x200;
        int offset_end = offset_begin + nsecs * 0x200;
        int offset = 0;
        int ret,disk_offset;
        const int Addr=0x13000000;
        const int rw=0;
        syscall_write_dev(&diskno,Addr+0x10,4);
        while (offset_begin + offset < offset_end) {
                disk_offset=offset_begin+offset;

                syscall_write_dev(&disk_offset,Addr+0x0,4);
                syscall_write_dev(&rw,Addr+0x20,4);

                syscall_read_dev(&ret,Addr+0x30,4);
                if(ret==0){
                        user_panic("IO failed because of some fuck things!\n");
                }
                syscall_read_dev(dst+offset,Addr+0x4000,0x200);
                offset+=0x200;
            // Your code here
            // error occurred, then panic.
        }
}


// Overview:
//      write data to IDE disk.
//
// Parameters:
//      diskno: disk number.
//      secno: start sector number.
//      src: the source data to write into IDE disk.
//      nsecs: the number of sectors to write.
//
// Post-Condition:
//      If error occurred during read the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
        int offset_begin = secno*0x200;
        int offset_end = offset_begin+nsecs*0x200;
        int offset = 0;
        writef("diskno: %d\n", diskno);
        int ret,disk_offset;
        const int rw=1;
        const int Addr=0x13000000;
        syscall_write_dev(&diskno,Addr+0x10,4);
        while (offset_begin+offset < offset_end ) {
            // copy data from source array to disk buffer.
                disk_offset=offset_begin+offset;
                syscall_write_dev(src+offset,Addr+0x4000,0x200);

                syscall_write_dev(&disk_offset,Addr+0x0,4);
                syscall_write_dev(&rw,Addr+0x20,4);

                syscall_read_dev(&ret,Addr+0x30,4);
                if(ret==0){
                        user_panic("IO failed again!!\n");
                }
                offset+=0x200;
            // if error occur, then panic.
        }
}


