// $Id$

#ifndef HTIOP_CONNECTOR_IMPL_CPP
#define HTIOP_CONNECTOR_IMPL_CPP

#include "HTIOP_Connector_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


ACE_RCSID(HTIOP,
          TAO_HTIOP_Connector_Impl,
          "$Id$")


template <class SVC_HANDLER>
TAO::HTIOP::Connect_Creation_Strategy<SVC_HANDLER>::
Connect_Creation_Strategy (ACE_Thread_Manager* t,
                                     TAO_ORB_Core *orb_core,
                                     void *arg,
                                     CORBA::Boolean flag)
  : ACE_Creation_Strategy <SVC_HANDLER> (t),
    orb_core_ (orb_core),
    arg_ (arg),
    lite_flag_ (flag)
{

}

template <class SVC_HANDLER> int
TAO::HTIOP::Connect_Creation_Strategy<SVC_HANDLER>::make_svc_handler (SVC_HANDLER *&sh)
{
  if (sh == 0)
    ACE_NEW_RETURN (sh,
                    SVC_HANDLER (this->orb_core_,
                                 this->lite_flag_,
                                 this->arg_),
                    -1);

  // We add to the #REFCOUNT# since the Connector needs this. See
  // Connector::make_connection() for details.
  sh->add_reference ();

  return 0;
}


////////////////////////////////////////////////////////////////

template <class SVC_HANDLER>
TAO::HTIOP::Connect_Concurrency_Strategy<SVC_HANDLER>::
Connect_Concurrency_Strategy (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

template <class SVC_HANDLER> int
TAO::HTIOP::Connect_Concurrency_Strategy<SVC_HANDLER>::
activate_svc_handler (SVC_HANDLER *sh,
                      void *arg)
{
  return ACE_Concurrency_Strategy<SVC_HANDLER>::activate_svc_handler (sh,
                                                                      arg);
}


#endif /* HTIOP_CONNECTOR_IMPL_CPP */
