#ifndef OPENJ2K_H
#define OPENJ2K_H 

constexpr auto J2K_DATA_MAGIC_NUMBER = 0x6972756B;

#pragma pack(push)
#pragma pack(2)

struct j2k_data_struct {
  //unsigned long m_MagicNumber;         /* Magic Number                 4 bytes, must be 0x6972756B */
  unsigned long m_EntryID;               /* entry id                     4 bytes */
  unsigned char m_Flag;                  /* general purpose bit flag     2 bytes */
  unsigned char m_WordSize;              /* word length                  2 bytes */
  unsigned char m_PumsaSize;             /* pumsa length                 2 bytes */
  unsigned char m_PropertyTypeASize;     /* property type A length       2 bytes */
  unsigned char m_PropertyTypeBSize;     /* property type B length       2 bytes */
  unsigned char m_DictionaryTypeSize;    /* dictionary type length       2 bytes */
  unsigned long m_NextEntry;             /* next entry id                4 bytes */
  unsigned long m_ExtraParam;            /* extra param                  4 bytes */

  /* word (variable size) */
  /* pumsa (variable size) */
  /* property type a (variable size) */
  /* property type b (variable size) */
  /* dict word (variable size) */
};
#pragma pack(pop)

#endif
