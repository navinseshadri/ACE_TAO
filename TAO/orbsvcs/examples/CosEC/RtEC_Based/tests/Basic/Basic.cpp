/* -*- C++ -*- */
// $Id$

#include "Basic.h"
#include "CosEvent_Utilities.h"

int
main (int argc, char *argv [])
{
  Basic basic;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      basic.init (argc, argv ACE_ENV_ARG_PARAMETER);

      basic.run ();

      basic.shutdown ();
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "CosEC_Basic user exception: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "CosEC_Basic system exception: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

Basic::Basic (void)
{
  // No-Op.
}

Basic::~Basic (void)
{
  // No-Op.
}

void
Basic::init (int argc, char *argv[]
                   ACE_ENV_ARG_DECL)
{
  this->init_ORB (argc, argv ACE_ENV_ARG_PARAMETER);

  this->init_CosEC ();
}

void
Basic::init_ORB  (int argc, char *argv []
                        ACE_ENV_ARG_DECL)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                ""
                                ACE_ENV_ARG_PARAMETER);

  CORBA::Object_var poa_object  =
    this->orb_->resolve_initial_references("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (poa_object.in ()))
    ACE_ERROR ((LM_ERROR,
                " (%P|%t) Unable to initialize the POA.\n"));

  root_poa_ =
    PortableServer::POA::_narrow (poa_object.in ()
                                  ACE_ENV_ARG_PARAMETER);

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager ();

  poa_manager->activate ();
}

void
Basic::init_CosEC (void)
{
  CosEC_ServantBase *ec = 0;

  ACE_NEW_THROW_EX (ec,
                    CosEC_ServantBase (),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var ec_var (ec);

  ec->init (this->root_poa_.in(),
            this->root_poa_.in(),
            0,0,0
            ACE_ENV_ARG_PARAMETER);

  int retval = ec->activate ();

  if (retval == -1)
    ACE_THROW (CORBA::UNKNOWN ());
  // @@ look for more descriptive exception to throw here

  CORBA::Object_var obj =
    this->root_poa_->servant_to_reference (ec ACE_ENV_ARG_PARAMETER);

  this->cos_ec_ =
    CosEventChannelAdmin::EventChannel::_narrow (obj._retn ()
                                                 ACE_ENV_ARG_PARAMETER);
}

void
Basic::run (void)
{
  // Create an Any type to pass to the Cos EC.
  CORBA::Any any;
  any <<= (CORBA::Long)50;

  this->consumer_.open (this->cos_ec_.in (),
                        this->orb_.in ()
                        ACE_ENV_ARG_PARAMETER);

  this->consumer_.connect ();

  this->supplier_.open (this->cos_ec_.in ()
                        ACE_ENV_ARG_PARAMETER);

  this->supplier_.connect ();

  this->supplier_.send_event (any
                              ACE_ENV_ARG_PARAMETER);

  // this->orb_->run ();
  // @@ commenting out the run-shutdown mechanism for now because it gives
  // an poa "unknown location" exception otherwise.

  // wait here.
  // When the consumer gets the event we pushed, it will shutdown the ORB.
  // We will return from <run> then.
}

void
Basic::shutdown (void)
{
  this->supplier_.close ();

  this->consumer_.close ();

  this->cos_ec_->destroy ();
}
