// $Id$

#include "orbsvcs/Notify/Any/CosEC_ProxyPushSupplier.h"

ACE_RCSID (Notify, TAO_Notify_CosEC_ProxyPushSupplier, "$Id$")

#include "tao/debug.h"
#include "orbsvcs/Notify/Any/PushConsumer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_CosEC_ProxyPushSupplier::TAO_Notify_CosEC_ProxyPushSupplier (void)
{
}

TAO_Notify_CosEC_ProxyPushSupplier::~TAO_Notify_CosEC_ProxyPushSupplier ()
{
}

void
TAO_Notify_CosEC_ProxyPushSupplier::release (void)
{

  delete this;
  //@@ inform factory
}

void
TAO_Notify_CosEC_ProxyPushSupplier::connect_push_consumer (CosEventComm::PushConsumer_ptr push_consumer
                                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected,
                   CosEventChannelAdmin::TypeError
                   ))
{
  // Convert Consumer to Base Type
  TAO_Notify_PushConsumer* consumer;
  ACE_NEW_THROW_EX (consumer,
                    TAO_Notify_PushConsumer (this),
                    CORBA::NO_MEMORY ());

  consumer->init (push_consumer ACE_ENV_ARG_PARAMETER);

  this->connect (consumer ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushSupplier::disconnect_push_supplier (void)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_CosEC_ProxyPushSupplier::Ptr guard( this );
  this->destroy ();
}

const char *
TAO_Notify_CosEC_ProxyPushSupplier::get_proxy_type_name (void) const
{
  return "ec_proxy_push_supplier";
}

TAO_END_VERSIONED_NAMESPACE_DECL
