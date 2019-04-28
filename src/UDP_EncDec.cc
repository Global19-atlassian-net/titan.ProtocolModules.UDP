/******************************************************************************
* Copyright (c) 2000-2019 Ericsson Telecom AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v2.0
* which accompanies this distribution, and is available at
* https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*
* Contributors:
*  Peter Kremer - initial implementation and initial documentation
*  Attila Balasko
*  Gabor Bettesch
*  Gabor Szalai
******************************************************************************/
//
//  File:               UDP_EncDec.cc
//  Description:        UDP
//  Rev:                R4A
//  Prodnr:             CNL 113 420
//
#include "UDP_Types.hh"
#include "General_Types.hh"
#include <netinet/in.h>

namespace UDP__Types {

TTCN_Module UDP__EncDec ( "UDP__EncDec", __DATE__, __TIME__ );

OCTETSTRING f__UDP__enc ( UDP__packet const &pdu )
{
  TTCN_Buffer bb;
  
  pdu.encode ( UDP__packet_descr_, bb, TTCN_EncDec::CT_RAW );
  
  return OCTETSTRING ( bb.get_len(), bb.get_data() );
}

OCTETSTRING f__UDP__pseudo__header__enc ( UDP__pseudo__header const &pdu )
{
  TTCN_Buffer bb;
  
  pdu.encode ( UDP__pseudo__header_descr_, bb, TTCN_EncDec::CT_RAW );
  
  return OCTETSTRING ( bb.get_len(), bb.get_data() );
}

UDP__packet f__UDP__dec ( OCTETSTRING const &data )
{
  const unsigned char *raw_data = (const unsigned char*)data;
  int data_length = data.lengthof();
  UDP__packet pdu;
  TTCN_Buffer bb;
  
  bb.clear();
  bb.put_s ( data_length,raw_data );
  
  pdu.decode ( UDP__packet_descr_, bb, TTCN_EncDec::CT_RAW );
  return pdu;
}

General__Types::OCT2 f__UDP__checksum ( OCTETSTRING const &data )
{
  unsigned short *stemp;
  unsigned short ret_val;
  unsigned long sum = 0;
  int datalen = data.lengthof();
  
  stemp = (unsigned short*)(const unsigned char *)data;
  
  for ( int i = 0; i < ( datalen / 2 ); i++ ) sum += htons ( stemp[i] );
  
  if ( datalen % 2 ) sum += ((const unsigned char *)data)[datalen-1] << 8;
  
  while (sum>>16){
  		sum = (sum & 0xFFFF)+(sum >> 16);
  }
  ret_val = htons ( ~(unsigned short)sum );
  /* Value 0 must be transferred as 0xffff, the 0 second one's complement
    representation. */
 // if (ret_val == 0) ret_val = 0xffff;
  
  return OCTETSTRING ( 2, (const unsigned char *)&ret_val );
}

}//namespace
