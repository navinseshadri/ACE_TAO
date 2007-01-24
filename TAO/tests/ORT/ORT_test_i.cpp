#include "ORT_test_i.h"


ACE_RCSID (ORT,
           ORT_test_i,
           "$Id$")


ORT_test_i::ORT_test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Boolean
ORT_test_i::request_server (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
ORT_test_i::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
}
