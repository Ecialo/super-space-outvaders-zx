// len(ugds) ==  512
const uint8_t compact_arr[] = {0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x3c, 0x23, 0x23, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x23, 0x23, 0x3c, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0xff, 0xff, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x21, 0x3f, 0x3f, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xe0, 0xe0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20};