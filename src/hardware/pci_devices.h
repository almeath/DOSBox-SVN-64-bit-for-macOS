/*
 *  Copyright (C) 2002-2021  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

class PCI_SSTDevice:public PCI_Device {
private:
	static const Bit16u vendor=0x121a;	// 3dfx
	Bit16u oscillator_ctr;
	Bit16u pci_ctr;
public:
	PCI_SSTDevice(Bitu type):PCI_Device(vendor,(type==2)?0x0002:0x0001) {
		oscillator_ctr=0;
		pci_ctr=0;
	}

	static Bit16u VendorID(void) { return vendor; }

	Bits ParseReadRegister(Bit8u regnum) {
//		LOG_MSG("SST ParseReadRegister %x",regnum);
		switch (regnum) {
			case 0x4c:
			case 0x4d:
			case 0x4e:
			case 0x4f:
				LOG_MSG("SST ParseReadRegister STATUS %x",regnum);
				break;
			case 0x54:
			case 0x55:
			case 0x56:
			case 0x57:
				if (DeviceID() >= 2) return -1;
				break;
			default:
				break;
		}
		return regnum;
	}

	bool OverrideReadRegister(Bit8u regnum, Bit8u* rval, Bit8u* rval_mask) {
		switch (regnum) {
			case 0x54:
				if (DeviceID() >= 2) {
					oscillator_ctr++;
					pci_ctr--;
					*rval=(oscillator_ctr | ((pci_ctr<<16) & 0x0fff0000)) & 0xff;
					*rval_mask=0xff;
					return true;
				}
				break;
			case 0x55:
				if (DeviceID() >= 2) {
					*rval=((oscillator_ctr | ((pci_ctr<<16) & 0x0fff0000)) >> 8) & 0xff;
					*rval_mask=0xff;
					return true;
				}
				break;
			case 0x56:
				if (DeviceID() >= 2) {
					*rval=((oscillator_ctr | ((pci_ctr<<16) & 0x0fff0000)) >> 16) & 0xff;
					*rval_mask=0xff;
					return true;
				}
				break;
			case 0x57:
				if (DeviceID() >= 2) {
					*rval=((oscillator_ctr | ((pci_ctr<<16) & 0x0fff0000)) >> 24) & 0xff;
					*rval_mask=0x0f;
					return true;
				}
				break;
			default:
				break;
		}
		return false;
	}

	Bits ParseWriteRegister(Bit8u regnum,Bit8u value) {
//		LOG_MSG("SST ParseWriteRegister %x:=%x",regnum,value);
		if ((regnum>=0x14) && (regnum<0x28)) return -1;	// base addresses are read-only
		if ((regnum>=0x30) && (regnum<0x34)) return -1;	// expansion rom addresses are read-only
		switch (regnum) {
			case 0x10:
				return (pci_cfg_data[this->PCIId()][this->PCISubfunction()][0x10]&0x0f);
			case 0x11:
				return 0x00;
			case 0x12:
				return (value&0x00);	// -> 16mb addressable (whyever)
			case 0x13:
				VOODOO_PCI_SetLFB(value<<24);
				return value;
			case 0x40:
				VOODOO_PCI_InitEnable(value&7);
				break;
			case 0x41:
			case 0x42:
			case 0x43:
				return -1;
			case 0xc0:
				VOODOO_PCI_Enable(true);
				return -1;
			case 0xe0:
				VOODOO_PCI_Enable(false);
				return -1;
			default:
				break;
		}
		return value;
	}

	bool InitializeRegisters(Bit8u registers[256]) {
		// init (3dfx voodoo)
		registers[0x08] = 0x02;	// revision
		registers[0x09] = 0x00;	// interface
//		registers[0x0a] = 0x00;	// subclass code
		registers[0x0a] = 0x00;	// subclass code (video/graphics controller)
//		registers[0x0b] = 0x00;	// class code (generic)
		registers[0x0b] = 0x04;	// class code (multimedia device)
		registers[0x0e] = 0x00;	// header type (other)

		// reset
		registers[0x04] = 0x02;	// command register (memory space enabled)
		registers[0x05] = 0x00;
		registers[0x06] = 0x80;	// status register (fast back-to-back)
		registers[0x07] = 0x00;

		registers[0x3c] = 0xff;	// no irq

		// memBaseAddr: size is 16MB
		Bit32u address_space=(((Bit32u)VOODOO_INITIAL_LFB)&0xfffffff0) | 0x08;	// memory space, within first 4GB, prefetchable
		registers[0x10] = (Bit8u)(address_space&0xff);		// base addres 0
		registers[0x11] = (Bit8u)((address_space>>8)&0xff);
		registers[0x12] = (Bit8u)((address_space>>16)&0xff);
		registers[0x13] = (Bit8u)((address_space>>24)&0xff);

		if (DeviceID() >= 2) {
			registers[0x40] = 0x00;
			registers[0x41] = 0x40;	// voodoo2 revision ID (rev4)
			registers[0x42] = 0x01;
			registers[0x43] = 0x00;
		}

		return true;
	}
};
