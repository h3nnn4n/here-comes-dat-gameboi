#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "memory.h"
#include "microcode.h"
#include "time_keeper.h"

#include "instructions_data_transfer.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
      (byte & 0x80 ? '1' : '0'), \
      (byte & 0x40 ? '1' : '0'), \
      (byte & 0x20 ? '1' : '0'), \
      (byte & 0x10 ? '1' : '0'), \
      (byte & 0x08 ? '1' : '0'), \
      (byte & 0x04 ? '1' : '0'), \
      (byte & 0x02 ? '1' : '0'), \
      (byte & 0x01 ? '1' : '0')

uint8_t *get_reg_ref ( _cpu_info *cpu, uint8_t opcode ) {
    switch ( opcode ) {
        case 0x00: // B
            return &(cpu->b);
        case 0x01: // C
            return &(cpu->c);
        case 0x02: // D
            return &(cpu->d);
        case 0x03: // E
            return &(cpu->e);
        case 0x04: // H
            return &(cpu->h);
        case 0x05: // L
            return &(cpu->l);
        case 0x06: // (HL)
            return &cpu->mem_controller.memory[( cpu->h << 8 ) | cpu->l];
        case 0x07: // A
            return &(cpu->a);
        default:
            printf(" -> %02x = 0b"BYTE_TO_BINARY_PATTERN"\n", opcode, BYTE_TO_BINARY(opcode));
            assert( 0 && "Invalid OPCODE for MOV");
    }
    return NULL;
}

void emulate_MOV ( _cpu_info *cpu ) {
    uint16_t addr;

    switch ( cpu->opcode ) {
        case 0x46: // MOV B, M
            addr = cpu->h << 8 | cpu->l;
            cpu->b = read_byte_with_tick ( cpu, addr );
            break;
        case 0x56: // MOV D, M
            addr = cpu->h << 8 | cpu->l;
            cpu->d = read_byte_with_tick ( cpu, addr );
            break;
        case 0x66: // MOV H, M
            addr = cpu->h << 8 | cpu->l;
            cpu->h = read_byte_with_tick ( cpu, addr );
            break;

        case 0x70: // MOV M, B
            addr = cpu->h << 8 | cpu->l;
            write_byte_with_tick ( cpu, addr, cpu->b );
            break;
        case 0x71: // MOV M, C
            addr = cpu->h << 8 | cpu->l;
            write_byte_with_tick ( cpu, addr, cpu->c );
            break;
        case 0x72: // MOV M, D
            addr = cpu->h << 8 | cpu->l;
            write_byte_with_tick ( cpu, addr, cpu->d );
            break;
        case 0x73: // MOV M, E
            addr = cpu->h << 8 | cpu->l;
            write_byte_with_tick ( cpu, addr, cpu->e );
            break;
        case 0x74: // MOV M, H
            addr = cpu->h << 8 | cpu->l;
            write_byte_with_tick ( cpu, addr, cpu->h );
            break;
        case 0x75: // MOV M, L
            addr = cpu->h << 8 | cpu->l;
            write_byte_with_tick ( cpu, addr, cpu->l );
            break;
        case 0x77: // MOV M, A
            addr = cpu->h << 8 | cpu->l;
            write_byte_with_tick ( cpu, addr, cpu->a );
            break;

        case 0x4e: // MOV C, M
            addr = cpu->h << 8 | cpu->l;
            cpu->c = read_byte_with_tick ( cpu, addr );
            break;
        case 0x5e: // MOV E, M
            addr = cpu->h << 8 | cpu->l;
            cpu->e = read_byte_with_tick ( cpu, addr );
            break;
        case 0x6e: // MOV L, M
            addr = cpu->h << 8 | cpu->l;
            cpu->l = read_byte_with_tick ( cpu, addr );
            break;
        case 0x7e: // MOV A, M
            addr = cpu->h << 8 | cpu->l;
            cpu->a = read_byte_with_tick ( cpu, addr );
            break;
        default:
            {
                uint8_t *dst = get_reg_ref( cpu, ( cpu->opcode & 0x38 ) >> 3 );
                uint8_t *src = get_reg_ref( cpu, ( cpu->opcode & 0x07 ) >> 0 );
                *dst = *src;
            }
            break;
    }
}
