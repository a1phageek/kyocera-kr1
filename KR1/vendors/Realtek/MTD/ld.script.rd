OUTPUT_FORMAT("elf32-tradbigmips")
OUTPUT_ARCH(mips)
SECTIONS
{
  .data :
  {
  /*. = ALIGN(256);		 */
    __romfs_rd_start = .;
    *(.data)
    __romfs_rd_end = .;
  }
}
