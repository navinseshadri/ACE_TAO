// $Id$

#include "TestC.h"
#include "Client_Task.h"
#include "orbsvcs/FaultTolerance/FT_ClientService_Activate.h"

//////////////////////////////////////////////////////////////////////////

ACE_CString file_prefix("file://");

/// Constructor
Client_Task::Client_Task (CORBA::ORB_ptr corb,
  ACE_Thread_Manager *thr_mgr,
  const ACE_CString& collocated_ior_file,
  const ACE_CString& server_ior_file)
  : ACE_Task_Base (thr_mgr)
  , corb_ (CORBA::ORB::_duplicate (corb))
  , collocated_ior_file_(collocated_ior_file)
  , server_ior_file_(server_ior_file)
{
}

CORBA::Object_ptr
Client_Task::make_iogr (const char* domain_id, CORBA::ULongLong group_id, CORBA::ULong group_version ACE_ENV_ARG_DECL)
{
  CORBA::Object_var remote_server(
    corb_->string_to_object (ACE_CString(file_prefix + this->server_ior_file_).c_str()));

  if (CORBA::is_nil (remote_server.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Error: Remote server is not started.\n"));
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }

  CORBA::Object_var collocated_server(
    corb_->string_to_object (ACE_CString(file_prefix + this->collocated_ior_file_).c_str()));

  if (CORBA::is_nil (collocated_server.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Error: Collocated server is not started.\n"));
      ACE_TRY_THROW (CORBA::INTERNAL ());
    }

  FT::TagFTGroupTaggedComponent ft_tag_component;

  // Create the list
  TAO_IOP::TAO_IOR_Manipulation::IORList iors (1);
  iors.length (2);
  iors [0] = CORBA::Object::_duplicate (remote_server.ptr());
  iors [1] = CORBA::Object::_duplicate (collocated_server.ptr());

  CORBA::Object_var new_ref =
    this->iorm_->merge_iors (iors ACE_ENV_ARG_PARAMETER);

  // Property values

  // Major and Minor revision numbers
  ft_tag_component.component_version.major = (CORBA::Octet) 1;
  ft_tag_component.component_version.minor = (CORBA::Octet) 0;

  // Domain id
  ft_tag_component.group_domain_id = domain_id;

  // Object group id
  ft_tag_component.object_group_id = group_id;

  // Version
  ft_tag_component.object_group_ref_version = group_version;

  // Construct the IOGR Property class
  TAO_FT_IOGR_Property iogr_prop (ft_tag_component);

  // Set the property
  CORBA::Boolean retval = this->iorm_->set_property (&iogr_prop,
                                              new_ref.in ()
                                              ACE_ENV_ARG_PARAMETER);

  // Set the primary
  // See we are setting the second ior as the primary
  retval = this->iorm_->set_primary (&iogr_prop,
                              remote_server.in (),
                              new_ref.in ()
                              ACE_ENV_ARG_PARAMETER);

  return new_ref._retn ();
}

/// Thread entry point
int Client_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      // Get a ref to the IORManipulation object
      CORBA::Object_var IORM =
        corb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
        0
        ACE_ENV_ARG_PARAMETER);

      // Narrow
      this->iorm_ =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in() ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var iogr = make_iogr ("Domain_1", 1, 1 ACE_ENV_ARG_PARAMETER);

      CORBA::String_var iorgr_string =
        corb_->object_to_string (iogr.in () ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var object =
        corb_->string_to_object (iorgr_string.in () ACE_ENV_ARG_PARAMETER);

      Test_var server =
        Test::_narrow (object.in () ACE_ENV_ARG_PARAMETER);

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            "Object reference <%s> is nil\n", iorgr_string.in()), 1);
        }

      CORBA::Object_var remote_server(
        corb_->string_to_object (ACE_CString(file_prefix + this->server_ior_file_).c_str()));

      Test_var remote_server_as_test =
        Test::_narrow (remote_server.in () ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var collocated_server(
        corb_->string_to_object (ACE_CString(file_prefix + this->collocated_ior_file_).c_str()));

      Test_var collocated_server_as_test =
        Test::_narrow (collocated_server.in () ACE_ENV_ARG_PARAMETER);

      if (!collocated_server->_is_collocated())
        { // Collocation is disabled, just skip the test - it has no sense.
          ACE_DEBUG ((LM_ERROR, "Test has no sense, because collocation is disabled.\n") );
        }
      else
        {
          ACE_TRY_NEW_ENV
          {
            // Following call will fail if the problem is not fixed.

            // Because we are using TRANSIENT objects with the SYSTEM_ID policy
            // the object keys won't match so the POA won't be able to dispatch locally.
            // This wouldn't work with 'direct' collocation strategy but the default is 'through poa'.
            server->myMethod ();
          }
          ACE_CATCHANY
          {
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
              "Exception caught in client task:");
          }
          ACE_ENDTRY;
        }

      remote_server_as_test->shutdown();

      collocated_server_as_test->shutdown();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
        "Exception caught in client task:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
