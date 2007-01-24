// $Id$

#include "tao/corba.h"
#include "tao/IORTable/IORTable.h"
#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/OS_NS_string.h"
#include "ace/SString.h"

#include "HelloS.h"

namespace Test
{
class Hello_impl: virtual public POA_Test::Hello
{
public:
  void say_hello() ACE_THROW_SPEC ((CORBA::SystemException)) { };

};
}

int main(int argc, char* argv[])
{
  int status = 0;
  CORBA::ORB_var orb = CORBA::ORB::_nil();

  ACE_TRY_NEW_ENV
    {
      // Initialize the ORB
      orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);

      // create Hello object
      Test::Hello_impl hello_i;

      Test::Hello_var hello = hello_i._this ();

      // give our object a friendly name
      CORBA::Object_var iorTableObj =
          orb->resolve_initial_references ("IORTable");

      IORTable::Table_var iorTable
          = IORTable::Table::_narrow (iorTableObj.in ());

      CORBA::String_var ior_string = orb->object_to_string (hello.in ());

      iorTable->bind("hello", ior_string.in ());

      ACE_DEBUG ((LM_DEBUG, "Created binding of name 'hello' in IOR table for IOR:\n%s\n", ior_string.in ()));

      // Get the root POA
      CORBA::Object_var obj_root = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var rootPOA = PortableServer::POA::_narrow (obj_root.in ());

      // Activate the POA manager
      PortableServer::POAManager_var poaManager = rootPOA->the_POAManager ();

      poaManager->activate ();

      // try and access the object with its friendly name
      ACE_CString full_corbaloc (ior_string.in (), 0, 1);

      CORBA::ULong first_slash = full_corbaloc.find ("/", 0);

      ACE_CString friendly_corbaloc =
        full_corbaloc.substring (0,
                                  first_slash);

      friendly_corbaloc += "/hello";

      ACE_DEBUG ((LM_DEBUG, "Trying to access object with object ref:\n%s\n", friendly_corbaloc.c_str ()));

      CORBA::Object_var obj = orb->string_to_object (friendly_corbaloc.c_str ());

      TimeBase::TimeT timeout = 10000000;

      CORBA::Any timeout_any;
      timeout_any <<= timeout;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);

      policy_list[0] = orb->create_policy (
                              Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                              timeout_any);

      CORBA::Object_var rtt_obj = obj->_set_policy_overrides (
                                                  policy_list,
                                                  CORBA::SET_OVERRIDE);

      policy_list[0]->destroy();

      Test::Hello_var hello2 = Test::Hello::_narrow (rtt_obj.in ());

      if (CORBA::is_nil (hello2.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to narrow from "
                      "corbaloc with policy override\n"));
          status = 1;
        }
      else
        {
          hello2->say_hello ();

          ACE_DEBUG ((LM_DEBUG, "Test succeeded !!!\n"));
        }

      orb->destroy();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                          "Error - test failed - exception caught:");
      status = 1;
    }
  ACE_ENDTRY;

  return status;
}
