// u16 pciConfigReadWord (u8 bus, u8 slot, u8 func, u8 offset) {
//         u32 address;
//         u32 lbus  = (u32)bus;
//         u32 lslot = (u32)slot;
//         u32 lfunc = (u32)func;
//         u16 tmp = 0;
//
//         /* create configuration address as per Figure 1 */
//         address = (u32)((lbus << 16) | (lslot << 11) |
//                         (lfunc << 8) | (offset & 0xfc) | ((u32)0x80000000));
//
//         /* write out the address */
//         outw(0xCF8, address);
//
//         /* read in the data */
//         /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
//         tmp = (u16)((inw(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
//
//
//
//         return (tmp);
// }
//
// // The PCI vendor ID is 0x10EC and the device ID is 0x8139.
//
// u16 pciCheckVendor(u8 bus, u8 slot) {
//         u16 vendor, device;
//
//         /* Try and read the first configuration register.
//            Since there are no vendors that == 0xFFFF
//            it must be a non-existent device. */
//
//         vendor = pciConfigReadWord(bus,slot,0,0);
//         if (vendor != 0xFFFF) {
//                 device = pciConfigReadWord(bus,slot,0,2);
//         }
//         return (vendor);
// }
//
//
// void checkDevice(uint8_t bus, uint8_t device) {
//         u8 function = 0;
//
//         vendorID = getVendorID(bus, device, function);
//         if(vendorID == 0xFFFF) return;    // Device doesn't exist
//
//         checkFunction(bus, device, function);
//         headerType = getHeaderType(bus, device, function);
//
//         if( (headerType & 0x80) != 0) {
//                 /* It is a multi-function device, so check remaining functions */
//                 for(function = 1; function < 8; function++) {
//                         if(getVendorID(bus, device, function) != 0xFFFF) {
//                                 checkFunction(bus, device, function);
//                         }
//                 }
//         }
// }
//
// void checkAllBuses(void) {
//         u16 bus;
//         u8 device;
//
//         for(bus = 0; bus < 256; bus++) {
//                 for(device = 0; device < 32; device++) {
//                         checkDevice(bus, device);
//                 }
//         }
// }
