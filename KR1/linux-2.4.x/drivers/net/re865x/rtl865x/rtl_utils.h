
#ifndef RTL8651_UTILITY_H
#define RTL8651_UTILITY_H

int8 *_strncpy(int8 *dst0, const int8 *src0, int32 count);
int32 _strncmp(const int8 *s1, const int8 *s2, int32 n);
void	rtl8651_memcpy(void *dst,void*src,int32 len);
int32 isPowerOf2(uint32 number, uint32 *exponent);
int32 IpStrToAscii(const int8 *cp, uint32 *addr);
void IntToAscii(int32 n, int8 s[]);
 int AsciiToInt( char **s);
 int32 ether_ntoa_r(ether_addr_t *n, int8 *a);
int8 *inet_ntoa_r(ipaddr_t ipaddr, 	int8 *p);
void memDump (void *start, uint32 size, int8 * strHeader);
#endif /* RTL8651_UTILITY_H */



