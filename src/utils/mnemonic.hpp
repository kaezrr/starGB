#include <string>

std::string get_op(uint8_t opcode) {
    std::string op;
    switch (opcode) {
    case 0x00:
        op = "NOP";
        break;
    case 0x01:
        op = "LD BC n16";
        break;
    case 0x02:
        op = "LD BC A";
        break;
    case 0x03:
        op = "INC BC";
        break;
    case 0x04:
        op = "INC B";
        break;
    case 0x05:
        op = "DEC B";
        break;
    case 0x06:
        op = "LD B n8";
        break;
    case 0x07:
        op = "RLCA";
        break;
    case 0x08:
        op = "LD a16 SP";
        break;
    case 0x09:
        op = "ADD HL BC";
        break;
    case 0x0A:
        op = "LD A BC";
        break;
    case 0x0B:
        op = "DEC BC";
        break;
    case 0x0C:
        op = "INC C";
        break;
    case 0x0D:
        op = "DEC C";
        break;
    case 0x0E:
        op = "LD C n8";
        break;
    case 0x0F:
        op = "RRCA";
        break;
    case 0x10:
        op = "STOP n8";
        break;
    case 0x11:
        op = "LD DE n16";
        break;
    case 0x12:
        op = "LD DE A";
        break;
    case 0x13:
        op = "INC DE";
        break;
    case 0x14:
        op = "INC D";
        break;
    case 0x15:
        op = "DEC D";
        break;
    case 0x16:
        op = "LD D n8";
        break;
    case 0x17:
        op = "RLA";
        break;
    case 0x18:
        op = "JR e8";
        break;
    case 0x19:
        op = "ADD HL DE";
        break;
    case 0x1A:
        op = "LD A DE";
        break;
    case 0x1B:
        op = "DEC DE";
        break;
    case 0x1C:
        op = "INC E";
        break;
    case 0x1D:
        op = "DEC E";
        break;
    case 0x1E:
        op = "LD E n8";
        break;
    case 0x1F:
        op = "RRA";
        break;
    case 0x20:
        op = "JR NZ e8";
        break;
    case 0x21:
        op = "LD HL n16";
        break;
    case 0x22:
        op = "LD HL A";
        break;
    case 0x23:
        op = "INC HL";
        break;
    case 0x24:
        op = "INC H";
        break;
    case 0x25:
        op = "DEC H";
        break;
    case 0x26:
        op = "LD H n8";
        break;
    case 0x27:
        op = "DAA";
        break;
    case 0x28:
        op = "JR Z e8";
        break;
    case 0x29:
        op = "ADD HL HL";
        break;
    case 0x2A:
        op = "LD A HL";
        break;
    case 0x2B:
        op = "DEC HL";
        break;
    case 0x2C:
        op = "INC L";
        break;
    case 0x2D:
        op = "DEC L";
        break;
    case 0x2E:
        op = "LD L n8";
        break;
    case 0x2F:
        op = "CPL";
        break;
    case 0x30:
        op = "JR NC e8";
        break;
    case 0x31:
        op = "LD SP n16";
        break;
    case 0x32:
        op = "LD HL A";
        break;
    case 0x33:
        op = "INC SP";
        break;
    case 0x34:
        op = "INC HL";
        break;
    case 0x35:
        op = "DEC HL";
        break;
    case 0x36:
        op = "LD HL n8";
        break;
    case 0x37:
        op = "SCF";
        break;
    case 0x38:
        op = "JR C e8";
        break;
    case 0x39:
        op = "ADD HL SP";
        break;
    case 0x3A:
        op = "LD A HL";
        break;
    case 0x3B:
        op = "DEC SP";
        break;
    case 0x3C:
        op = "INC A";
        break;
    case 0x3D:
        op = "DEC A";
        break;
    case 0x3E:
        op = "LD A n8";
        break;
    case 0x3F:
        op = "CCF";
        break;
    case 0x40:
        op = "LD B B";
        break;
    case 0x41:
        op = "LD B C";
        break;
    case 0x42:
        op = "LD B D";
        break;
    case 0x43:
        op = "LD B E";
        break;
    case 0x44:
        op = "LD B H";
        break;
    case 0x45:
        op = "LD B L";
        break;
    case 0x46:
        op = "LD B HL";
        break;
    case 0x47:
        op = "LD B A";
        break;
    case 0x48:
        op = "LD C B";
        break;
    case 0x49:
        op = "LD C C";
        break;
    case 0x4A:
        op = "LD C D";
        break;
    case 0x4B:
        op = "LD C E";
        break;
    case 0x4C:
        op = "LD C H";
        break;
    case 0x4D:
        op = "LD C L";
        break;
    case 0x4E:
        op = "LD C HL";
        break;
    case 0x4F:
        op = "LD C A";
        break;
    case 0x50:
        op = "LD D B";
        break;
    case 0x51:
        op = "LD D C";
        break;
    case 0x52:
        op = "LD D D";
        break;
    case 0x53:
        op = "LD D E";
        break;
    case 0x54:
        op = "LD D H";
        break;
    case 0x55:
        op = "LD D L";
        break;
    case 0x56:
        op = "LD D HL";
        break;
    case 0x57:
        op = "LD D A";
        break;
    case 0x58:
        op = "LD E B";
        break;
    case 0x59:
        op = "LD E C";
        break;
    case 0x5A:
        op = "LD E D";
        break;
    case 0x5B:
        op = "LD E E";
        break;
    case 0x5C:
        op = "LD E H";
        break;
    case 0x5D:
        op = "LD E L";
        break;
    case 0x5E:
        op = "LD E HL";
        break;
    case 0x5F:
        op = "LD E A";
        break;
    case 0x60:
        op = "LD H B";
        break;
    case 0x61:
        op = "LD H C";
        break;
    case 0x62:
        op = "LD H D";
        break;
    case 0x63:
        op = "LD H E";
        break;
    case 0x64:
        op = "LD H H";
        break;
    case 0x65:
        op = "LD H L";
        break;
    case 0x66:
        op = "LD H HL";
        break;
    case 0x67:
        op = "LD H A";
        break;
    case 0x68:
        op = "LD L B";
        break;
    case 0x69:
        op = "LD L C";
        break;
    case 0x6A:
        op = "LD L D";
        break;
    case 0x6B:
        op = "LD L E";
        break;
    case 0x6C:
        op = "LD L H";
        break;
    case 0x6D:
        op = "LD L L";
        break;
    case 0x6E:
        op = "LD L HL";
        break;
    case 0x6F:
        op = "LD L A";
        break;
    case 0x70:
        op = "LD HL B";
        break;
    case 0x71:
        op = "LD HL C";
        break;
    case 0x72:
        op = "LD HL D";
        break;
    case 0x73:
        op = "LD HL E";
        break;
    case 0x74:
        op = "LD HL H";
        break;
    case 0x75:
        op = "LD HL L";
        break;
    case 0x76:
        op = "HALT";
        break;
    case 0x77:
        op = "LD HL A";
        break;
    case 0x78:
        op = "LD A B";
        break;
    case 0x79:
        op = "LD A C";
        break;
    case 0x7A:
        op = "LD A D";
        break;
    case 0x7B:
        op = "LD A E";
        break;
    case 0x7C:
        op = "LD A H";
        break;
    case 0x7D:
        op = "LD A L";
        break;
    case 0x7E:
        op = "LD A HL";
        break;
    case 0x7F:
        op = "LD A A";
        break;
    case 0x80:
        op = "ADD A B";
        break;
    case 0x81:
        op = "ADD A C";
        break;
    case 0x82:
        op = "ADD A D";
        break;
    case 0x83:
        op = "ADD A E";
        break;
    case 0x84:
        op = "ADD A H";
        break;
    case 0x85:
        op = "ADD A L";
        break;
    case 0x86:
        op = "ADD A HL";
        break;
    case 0x87:
        op = "ADD A A";
        break;
    case 0x88:
        op = "ADC A B";
        break;
    case 0x89:
        op = "ADC A C";
        break;
    case 0x8A:
        op = "ADC A D";
        break;
    case 0x8B:
        op = "ADC A E";
        break;
    case 0x8C:
        op = "ADC A H";
        break;
    case 0x8D:
        op = "ADC A L";
        break;
    case 0x8E:
        op = "ADC A HL";
        break;
    case 0x8F:
        op = "ADC A A";
        break;
    case 0x90:
        op = "SUB A B";
        break;
    case 0x91:
        op = "SUB A C";
        break;
    case 0x92:
        op = "SUB A D";
        break;
    case 0x93:
        op = "SUB A E";
        break;
    case 0x94:
        op = "SUB A H";
        break;
    case 0x95:
        op = "SUB A L";
        break;
    case 0x96:
        op = "SUB A HL";
        break;
    case 0x97:
        op = "SUB A A";
        break;
    case 0x98:
        op = "SBC A B";
        break;
    case 0x99:
        op = "SBC A C";
        break;
    case 0x9A:
        op = "SBC A D";
        break;
    case 0x9B:
        op = "SBC A E";
        break;
    case 0x9C:
        op = "SBC A H";
        break;
    case 0x9D:
        op = "SBC A L";
        break;
    case 0x9E:
        op = "SBC A HL";
        break;
    case 0x9F:
        op = "SBC A A";
        break;
    case 0xA0:
        op = "AND A B";
        break;
    case 0xA1:
        op = "AND A C";
        break;
    case 0xA2:
        op = "AND A D";
        break;
    case 0xA3:
        op = "AND A E";
        break;
    case 0xA4:
        op = "AND A H";
        break;
    case 0xA5:
        op = "AND A L";
        break;
    case 0xA6:
        op = "AND A HL";
        break;
    case 0xA7:
        op = "AND A A";
        break;
    case 0xA8:
        op = "XOR A B";
        break;
    case 0xA9:
        op = "XOR A C";
        break;
    case 0xAA:
        op = "XOR A D";
        break;
    case 0xAB:
        op = "XOR A E";
        break;
    case 0xAC:
        op = "XOR A H";
        break;
    case 0xAD:
        op = "XOR A L";
        break;
    case 0xAE:
        op = "XOR A HL";
        break;
    case 0xAF:
        op = "XOR A A";
        break;
    case 0xB0:
        op = "OR A B";
        break;
    case 0xB1:
        op = "OR A C";
        break;
    case 0xB2:
        op = "OR A D";
        break;
    case 0xB3:
        op = "OR A E";
        break;
    case 0xB4:
        op = "OR A H";
        break;
    case 0xB5:
        op = "OR A L";
        break;
    case 0xB6:
        op = "OR A HL";
        break;
    case 0xB7:
        op = "OR A A";
        break;
    case 0xB8:
        op = "CP A B";
        break;
    case 0xB9:
        op = "CP A C";
        break;
    case 0xBA:
        op = "CP A D";
        break;
    case 0xBB:
        op = "CP A E";
        break;
    case 0xBC:
        op = "CP A H";
        break;
    case 0xBD:
        op = "CP A L";
        break;
    case 0xBE:
        op = "CP A HL";
        break;
    case 0xBF:
        op = "CP A A";
        break;
    case 0xC0:
        op = "RET NZ";
        break;
    case 0xC1:
        op = "POP BC";
        break;
    case 0xC2:
        op = "JP NZ a16";
        break;
    case 0xC3:
        op = "JP a16";
        break;
    case 0xC4:
        op = "CALL NZ a16";
        break;
    case 0xC5:
        op = "PUSH BC";
        break;
    case 0xC6:
        op = "ADD A n8";
        break;
    case 0xC7:
        op = "RST $00";
        break;
    case 0xC8:
        op = "RET Z";
        break;
    case 0xC9:
        op = "RET";
        break;
    case 0xCA:
        op = "JP Z a16";
        break;
    case 0xCB:
        op = "PREFIX";
        break;
    case 0xCC:
        op = "CALL Z a16";
        break;
    case 0xCD:
        op = "CALL a16";
        break;
    case 0xCE:
        op = "ADC A n8";
        break;
    case 0xCF:
        op = "RST $08";
        break;
    case 0xD0:
        op = "RET NC";
        break;
    case 0xD1:
        op = "POP DE";
        break;
    case 0xD2:
        op = "JP NC a16";
        break;
    case 0xD3:
        op = "ILLEGAL_D3";
        break;
    case 0xD4:
        op = "CALL NC a16";
        break;
    case 0xD5:
        op = "PUSH DE";
        break;
    case 0xD6:
        op = "SUB A n8";
        break;
    case 0xD7:
        op = "RST $10";
        break;
    case 0xD8:
        op = "RET C";
        break;
    case 0xD9:
        op = "RETI";
        break;
    case 0xDA:
        op = "JP C a16";
        break;
    case 0xDB:
        op = "ILLEGAL_DB";
        break;
    case 0xDC:
        op = "CALL C a16";
        break;
    case 0xDD:
        op = "ILLEGAL_DD";
        break;
    case 0xDE:
        op = "SBC A n8";
        break;
    case 0xDF:
        op = "RST $18";
        break;
    case 0xE0:
        op = "LDH a8 A";
        break;
    case 0xE1:
        op = "POP HL";
        break;
    case 0xE2:
        op = "LD C A";
        break;
    case 0xE3:
        op = "ILLEGAL_E3";
        break;
    case 0xE4:
        op = "ILLEGAL_E4";
        break;
    case 0xE5:
        op = "PUSH HL";
        break;
    case 0xE6:
        op = "AND A n8";
        break;
    case 0xE7:
        op = "RST $20";
        break;
    case 0xE8:
        op = "ADD SP e8";
        break;
    case 0xE9:
        op = "JP HL";
        break;
    case 0xEA:
        op = "LD a16 A";
        break;
    case 0xEB:
        op = "ILLEGAL_EB";
        break;
    case 0xEC:
        op = "ILLEGAL_EC";
        break;
    case 0xED:
        op = "ILLEGAL_ED";
        break;
    case 0xEE:
        op = "XOR A n8";
        break;
    case 0xEF:
        op = "RST $28";
        break;
    case 0xF0:
        op = "LDH A a8";
        break;
    case 0xF1:
        op = "POP AF";
        break;
    case 0xF2:
        op = "LD A C";
        break;
    case 0xF3:
        op = "DI";
        break;
    case 0xF4:
        op = "ILLEGAL_F4";
        break;
    case 0xF5:
        op = "PUSH AF";
        break;
    case 0xF6:
        op = "OR A n8";
        break;
    case 0xF7:
        op = "RST $30";
        break;
    case 0xF8:
        op = "LD HL SP e8";
        break;
    case 0xF9:
        op = "LD SP HL";
        break;
    case 0xFA:
        op = "LD A a16";
        break;
    case 0xFB:
        op = "EI";
        break;
    case 0xFC:
        op = "ILLEGAL_FC";
        break;
    case 0xFD:
        op = "ILLEGAL_FD";
        break;
    case 0xFE:
        op = "CP A n8";
        break;
    case 0xFF:
        op = "RST $38";
        break;
    }
    return op;
}

std::string get_cb(uint8_t opcode) {
    std::string op;
    switch (opcode) {
    case 0x00:
        op = "RLC B";
        break;
    case 0x01:
        op = "RLC C";
        break;
    case 0x02:
        op = "RLC D";
        break;
    case 0x03:
        op = "RLC E";
        break;
    case 0x04:
        op = "RLC H";
        break;
    case 0x05:
        op = "RLC L";
        break;
    case 0x06:
        op = "RLC HL";
        break;
    case 0x07:
        op = "RLC A";
        break;
    case 0x08:
        op = "RRC B";
        break;
    case 0x09:
        op = "RRC C";
        break;
    case 0x0A:
        op = "RRC D";
        break;
    case 0x0B:
        op = "RRC E";
        break;
    case 0x0C:
        op = "RRC H";
        break;
    case 0x0D:
        op = "RRC L";
        break;
    case 0x0E:
        op = "RRC HL";
        break;
    case 0x0F:
        op = "RRC A";
        break;
    case 0x10:
        op = "RL B";
        break;
    case 0x11:
        op = "RL C";
        break;
    case 0x12:
        op = "RL D";
        break;
    case 0x13:
        op = "RL E";
        break;
    case 0x14:
        op = "RL H";
        break;
    case 0x15:
        op = "RL L";
        break;
    case 0x16:
        op = "RL HL";
        break;
    case 0x17:
        op = "RL A";
        break;
    case 0x18:
        op = "RR B";
        break;
    case 0x19:
        op = "RR C";
        break;
    case 0x1A:
        op = "RR D";
        break;
    case 0x1B:
        op = "RR E";
        break;
    case 0x1C:
        op = "RR H";
        break;
    case 0x1D:
        op = "RR L";
        break;
    case 0x1E:
        op = "RR HL";
        break;
    case 0x1F:
        op = "RR A";
        break;
    case 0x20:
        op = "SLA B";
        break;
    case 0x21:
        op = "SLA C";
        break;
    case 0x22:
        op = "SLA D";
        break;
    case 0x23:
        op = "SLA E";
        break;
    case 0x24:
        op = "SLA H";
        break;
    case 0x25:
        op = "SLA L";
        break;
    case 0x26:
        op = "SLA HL";
        break;
    case 0x27:
        op = "SLA A";
        break;
    case 0x28:
        op = "SRA B";
        break;
    case 0x29:
        op = "SRA C";
        break;
    case 0x2A:
        op = "SRA D";
        break;
    case 0x2B:
        op = "SRA E";
        break;
    case 0x2C:
        op = "SRA H";
        break;
    case 0x2D:
        op = "SRA L";
        break;
    case 0x2E:
        op = "SRA HL";
        break;
    case 0x2F:
        op = "SRA A";
        break;
    case 0x30:
        op = "SWAP B";
        break;
    case 0x31:
        op = "SWAP C";
        break;
    case 0x32:
        op = "SWAP D";
        break;
    case 0x33:
        op = "SWAP E";
        break;
    case 0x34:
        op = "SWAP H";
        break;
    case 0x35:
        op = "SWAP L";
        break;
    case 0x36:
        op = "SWAP HL";
        break;
    case 0x37:
        op = "SWAP A";
        break;
    case 0x38:
        op = "SRL B";
        break;
    case 0x39:
        op = "SRL C";
        break;
    case 0x3A:
        op = "SRL D";
        break;
    case 0x3B:
        op = "SRL E";
        break;
    case 0x3C:
        op = "SRL H";
        break;
    case 0x3D:
        op = "SRL L";
        break;
    case 0x3E:
        op = "SRL HL";
        break;
    case 0x3F:
        op = "SRL A";
        break;
    case 0x40:
        op = "BIT 0 B";
        break;
    case 0x41:
        op = "BIT 0 C";
        break;
    case 0x42:
        op = "BIT 0 D";
        break;
    case 0x43:
        op = "BIT 0 E";
        break;
    case 0x44:
        op = "BIT 0 H";
        break;
    case 0x45:
        op = "BIT 0 L";
        break;
    case 0x46:
        op = "BIT 0 HL";
        break;
    case 0x47:
        op = "BIT 0 A";
        break;
    case 0x48:
        op = "BIT 1 B";
        break;
    case 0x49:
        op = "BIT 1 C";
        break;
    case 0x4A:
        op = "BIT 1 D";
        break;
    case 0x4B:
        op = "BIT 1 E";
        break;
    case 0x4C:
        op = "BIT 1 H";
        break;
    case 0x4D:
        op = "BIT 1 L";
        break;
    case 0x4E:
        op = "BIT 1 HL";
        break;
    case 0x4F:
        op = "BIT 1 A";
        break;
    case 0x50:
        op = "BIT 2 B";
        break;
    case 0x51:
        op = "BIT 2 C";
        break;
    case 0x52:
        op = "BIT 2 D";
        break;
    case 0x53:
        op = "BIT 2 E";
        break;
    case 0x54:
        op = "BIT 2 H";
        break;
    case 0x55:
        op = "BIT 2 L";
        break;
    case 0x56:
        op = "BIT 2 HL";
        break;
    case 0x57:
        op = "BIT 2 A";
        break;
    case 0x58:
        op = "BIT 3 B";
        break;
    case 0x59:
        op = "BIT 3 C";
        break;
    case 0x5A:
        op = "BIT 3 D";
        break;
    case 0x5B:
        op = "BIT 3 E";
        break;
    case 0x5C:
        op = "BIT 3 H";
        break;
    case 0x5D:
        op = "BIT 3 L";
        break;
    case 0x5E:
        op = "BIT 3 HL";
        break;
    case 0x5F:
        op = "BIT 3 A";
        break;
    case 0x60:
        op = "BIT 4 B";
        break;
    case 0x61:
        op = "BIT 4 C";
        break;
    case 0x62:
        op = "BIT 4 D";
        break;
    case 0x63:
        op = "BIT 4 E";
        break;
    case 0x64:
        op = "BIT 4 H";
        break;
    case 0x65:
        op = "BIT 4 L";
        break;
    case 0x66:
        op = "BIT 4 HL";
        break;
    case 0x67:
        op = "BIT 4 A";
        break;
    case 0x68:
        op = "BIT 5 B";
        break;
    case 0x69:
        op = "BIT 5 C";
        break;
    case 0x6A:
        op = "BIT 5 D";
        break;
    case 0x6B:
        op = "BIT 5 E";
        break;
    case 0x6C:
        op = "BIT 5 H";
        break;
    case 0x6D:
        op = "BIT 5 L";
        break;
    case 0x6E:
        op = "BIT 5 HL";
        break;
    case 0x6F:
        op = "BIT 5 A";
        break;
    case 0x70:
        op = "BIT 6 B";
        break;
    case 0x71:
        op = "BIT 6 C";
        break;
    case 0x72:
        op = "BIT 6 D";
        break;
    case 0x73:
        op = "BIT 6 E";
        break;
    case 0x74:
        op = "BIT 6 H";
        break;
    case 0x75:
        op = "BIT 6 L";
        break;
    case 0x76:
        op = "BIT 6 HL";
        break;
    case 0x77:
        op = "BIT 6 A";
        break;
    case 0x78:
        op = "BIT 7 B";
        break;
    case 0x79:
        op = "BIT 7 C";
        break;
    case 0x7A:
        op = "BIT 7 D";
        break;
    case 0x7B:
        op = "BIT 7 E";
        break;
    case 0x7C:
        op = "BIT 7 H";
        break;
    case 0x7D:
        op = "BIT 7 L";
        break;
    case 0x7E:
        op = "BIT 7 HL";
        break;
    case 0x7F:
        op = "BIT 7 A";
        break;
    case 0x80:
        op = "RES 0 B";
        break;
    case 0x81:
        op = "RES 0 C";
        break;
    case 0x82:
        op = "RES 0 D";
        break;
    case 0x83:
        op = "RES 0 E";
        break;
    case 0x84:
        op = "RES 0 H";
        break;
    case 0x85:
        op = "RES 0 L";
        break;
    case 0x86:
        op = "RES 0 HL";
        break;
    case 0x87:
        op = "RES 0 A";
        break;
    case 0x88:
        op = "RES 1 B";
        break;
    case 0x89:
        op = "RES 1 C";
        break;
    case 0x8A:
        op = "RES 1 D";
        break;
    case 0x8B:
        op = "RES 1 E";
        break;
    case 0x8C:
        op = "RES 1 H";
        break;
    case 0x8D:
        op = "RES 1 L";
        break;
    case 0x8E:
        op = "RES 1 HL";
        break;
    case 0x8F:
        op = "RES 1 A";
        break;
    case 0x90:
        op = "RES 2 B";
        break;
    case 0x91:
        op = "RES 2 C";
        break;
    case 0x92:
        op = "RES 2 D";
        break;
    case 0x93:
        op = "RES 2 E";
        break;
    case 0x94:
        op = "RES 2 H";
        break;
    case 0x95:
        op = "RES 2 L";
        break;
    case 0x96:
        op = "RES 2 HL";
        break;
    case 0x97:
        op = "RES 2 A";
        break;
    case 0x98:
        op = "RES 3 B";
        break;
    case 0x99:
        op = "RES 3 C";
        break;
    case 0x9A:
        op = "RES 3 D";
        break;
    case 0x9B:
        op = "RES 3 E";
        break;
    case 0x9C:
        op = "RES 3 H";
        break;
    case 0x9D:
        op = "RES 3 L";
        break;
    case 0x9E:
        op = "RES 3 HL";
        break;
    case 0x9F:
        op = "RES 3 A";
        break;
    case 0xA0:
        op = "RES 4 B";
        break;
    case 0xA1:
        op = "RES 4 C";
        break;
    case 0xA2:
        op = "RES 4 D";
        break;
    case 0xA3:
        op = "RES 4 E";
        break;
    case 0xA4:
        op = "RES 4 H";
        break;
    case 0xA5:
        op = "RES 4 L";
        break;
    case 0xA6:
        op = "RES 4 HL";
        break;
    case 0xA7:
        op = "RES 4 A";
        break;
    case 0xA8:
        op = "RES 5 B";
        break;
    case 0xA9:
        op = "RES 5 C";
        break;
    case 0xAA:
        op = "RES 5 D";
        break;
    case 0xAB:
        op = "RES 5 E";
        break;
    case 0xAC:
        op = "RES 5 H";
        break;
    case 0xAD:
        op = "RES 5 L";
        break;
    case 0xAE:
        op = "RES 5 HL";
        break;
    case 0xAF:
        op = "RES 5 A";
        break;
    case 0xB0:
        op = "RES 6 B";
        break;
    case 0xB1:
        op = "RES 6 C";
        break;
    case 0xB2:
        op = "RES 6 D";
        break;
    case 0xB3:
        op = "RES 6 E";
        break;
    case 0xB4:
        op = "RES 6 H";
        break;
    case 0xB5:
        op = "RES 6 L";
        break;
    case 0xB6:
        op = "RES 6 HL";
        break;
    case 0xB7:
        op = "RES 6 A";
        break;
    case 0xB8:
        op = "RES 7 B";
        break;
    case 0xB9:
        op = "RES 7 C";
        break;
    case 0xBA:
        op = "RES 7 D";
        break;
    case 0xBB:
        op = "RES 7 E";
        break;
    case 0xBC:
        op = "RES 7 H";
        break;
    case 0xBD:
        op = "RES 7 L";
        break;
    case 0xBE:
        op = "RES 7 HL";
        break;
    case 0xBF:
        op = "RES 7 A";
        break;
    case 0xC0:
        op = "SET 0 B";
        break;
    case 0xC1:
        op = "SET 0 C";
        break;
    case 0xC2:
        op = "SET 0 D";
        break;
    case 0xC3:
        op = "SET 0 E";
        break;
    case 0xC4:
        op = "SET 0 H";
        break;
    case 0xC5:
        op = "SET 0 L";
        break;
    case 0xC6:
        op = "SET 0 HL";
        break;
    case 0xC7:
        op = "SET 0 A";
        break;
    case 0xC8:
        op = "SET 1 B";
        break;
    case 0xC9:
        op = "SET 1 C";
        break;
    case 0xCA:
        op = "SET 1 D";
        break;
    case 0xCB:
        op = "SET 1 E";
        break;
    case 0xCC:
        op = "SET 1 H";
        break;
    case 0xCD:
        op = "SET 1 L";
        break;
    case 0xCE:
        op = "SET 1 HL";
        break;
    case 0xCF:
        op = "SET 1 A";
        break;
    case 0xD0:
        op = "SET 2 B";
        break;
    case 0xD1:
        op = "SET 2 C";
        break;
    case 0xD2:
        op = "SET 2 D";
        break;
    case 0xD3:
        op = "SET 2 E";
        break;
    case 0xD4:
        op = "SET 2 H";
        break;
    case 0xD5:
        op = "SET 2 L";
        break;
    case 0xD6:
        op = "SET 2 HL";
        break;
    case 0xD7:
        op = "SET 2 A";
        break;
    case 0xD8:
        op = "SET 3 B";
        break;
    case 0xD9:
        op = "SET 3 C";
        break;
    case 0xDA:
        op = "SET 3 D";
        break;
    case 0xDB:
        op = "SET 3 E";
        break;
    case 0xDC:
        op = "SET 3 H";
        break;
    case 0xDD:
        op = "SET 3 L";
        break;
    case 0xDE:
        op = "SET 3 HL";
        break;
    case 0xDF:
        op = "SET 3 A";
        break;
    case 0xE0:
        op = "SET 4 B";
        break;
    case 0xE1:
        op = "SET 4 C";
        break;
    case 0xE2:
        op = "SET 4 D";
        break;
    case 0xE3:
        op = "SET 4 E";
        break;
    case 0xE4:
        op = "SET 4 H";
        break;
    case 0xE5:
        op = "SET 4 L";
        break;
    case 0xE6:
        op = "SET 4 HL";
        break;
    case 0xE7:
        op = "SET 4 A";
        break;
    case 0xE8:
        op = "SET 5 B";
        break;
    case 0xE9:
        op = "SET 5 C";
        break;
    case 0xEA:
        op = "SET 5 D";
        break;
    case 0xEB:
        op = "SET 5 E";
        break;
    case 0xEC:
        op = "SET 5 H";
        break;
    case 0xED:
        op = "SET 5 L";
        break;
    case 0xEE:
        op = "SET 5 HL";
        break;
    case 0xEF:
        op = "SET 5 A";
        break;
    case 0xF0:
        op = "SET 6 B";
        break;
    case 0xF1:
        op = "SET 6 C";
        break;
    case 0xF2:
        op = "SET 6 D";
        break;
    case 0xF3:
        op = "SET 6 E";
        break;
    case 0xF4:
        op = "SET 6 H";
        break;
    case 0xF5:
        op = "SET 6 L";
        break;
    case 0xF6:
        op = "SET 6 HL";
        break;
    case 0xF7:
        op = "SET 6 A";
        break;
    case 0xF8:
        op = "SET 7 B";
        break;
    case 0xF9:
        op = "SET 7 C";
        break;
    case 0xFA:
        op = "SET 7 D";
        break;
    case 0xFB:
        op = "SET 7 E";
        break;
    case 0xFC:
        op = "SET 7 H";
        break;
    case 0xFD:
        op = "SET 7 L";
        break;
    case 0xFE:
        op = "SET 7 HL";
        break;
    case 0xFF:
        op = "SET 7 A";
        break;
    }
    return op;
}
