// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be/be_codegen.cpp:923

#ifndef INTEROP_WCHAR_I_H_
#define INTEROP_WCHAR_I_H_

#include "interop_wcharS.h"
#include "wchar_reference.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//Class interop_WChar_Passer_i
class  interop_WChar_Passer_i : public virtual POA_interop::WChar_Passer
{
public:
  //Constructor
  interop_WChar_Passer_i (CORBA::ORB_ptr o, int verbose);

  //Destructor
  virtual ~interop_WChar_Passer_i (void);

  virtual char * orb_name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Boolean wchar_to_server (CORBA::WChar test,
                                          CORBA::Short key
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::WChar wchar_from_server (CORBA::Short key
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Boolean wstring_to_server (const CORBA::WChar * test,
                                            CORBA::Short key
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::WChar * wstring_from_server (CORBA::Short key
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Boolean warray_to_server (const interop::warray test,
                                           CORBA::Short key
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual interop::warray_slice * warray_from_server (CORBA::Short key
                                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Boolean wstruct_to_server (const interop::wstruct & test,
                                            CORBA::Short key
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual interop::wstruct * wstruct_from_server (CORBA::Short key
                                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Boolean wunion_to_server (const interop::wunion & test,
                                           CORBA::Short key
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual interop::wunion * wunion_from_server (CORBA::Short key,
                                                interop::wchar_types type
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Boolean any_to_server (const CORBA::Any &test,
                                        CORBA::Short key
                                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Any* any_from_server (CORBA::Short key,
                                       interop::wchar_types type
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC (( CORBA::SystemException ));

  virtual CORBA::Any* any_echo (const CORBA::Any &test
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC (( CORBA::SystemException ));

virtual void exception_test (CORBA::Short key
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC (( CORBA::SystemException,
                    interop::WChar_Passer::WStringException ));

virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC (( CORBA::SystemException ));

private:
  CORBA::ORB_var orb_;
  wchar_reference ref_;

};


#endif /* INTEROP_WCHAR_I_H_  */
