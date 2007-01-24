// $Id$

#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "ace/Read_Buffer.h"
#include "testC.h"

ACE_RCSID(Generic_Servant, client, "$Id$")

static char *IOR = 0;
static int iterations = 1;
static int oneway = 0;
static int shutdown_server = 0;
static CORBA::ULong timeout = 5;
static int timed_method = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:k:i:T:otx");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'o':
        oneway = 1;
        break;

      case 't':
        timed_method = 1;
        break;

      case 'i':
        iterations = ::atoi (get_opts.opt_arg ());
        break;

      case 'T':
        timeout = static_cast<CORBA::ULong> (::atoi (get_opts.opt_arg ()));
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-o oneway "
                           "-t timed operations "
                           "-T timeout for timed operations "
                           "-i iterations "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

void
print_stats (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time,
             int iterations)
{
  if (iterations > 0)
    {
      elapsed_time.real_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.user_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.system_time *= ACE_ONE_SECOND_IN_MSECS;

      elapsed_time.real_time /= iterations;
      elapsed_time.user_time /= iterations;
      elapsed_time.system_time /= iterations;

      double tmp = 1000 / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
                  "\titerations\t = %d, \n"
                  "\treal_time\t = %0.06f ms, \n"
                  "\tuser_time\t = %0.06f ms, \n"
                  "\tsystem_time\t = %0.06f ms, \n"
                  "\t%0.00f calls/second\n",
                  iterations,
                  elapsed_time.real_time   < 0.0 ? 0.0 : elapsed_time.real_time,
                  elapsed_time.user_time   < 0.0 ? 0.0 : elapsed_time.user_time,
                  elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time,
                  tmp < 0.0 ? 0.0 : tmp));
    }
  else
    ACE_ERROR ((LM_ERROR,
                "\tNo time stats printed.  Zero iterations or error ocurred.\n"));
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         ACE_ENV_ARG_PARAMETER);

      // Initialize options based on command-line arguments.
      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var object =
        orb->string_to_object (IOR
                               ACE_ENV_ARG_PARAMETER);

      // Try to narrow the object reference to a test reference.
      test_var test = test::_narrow (object.in ()
                                     ACE_ENV_ARG_PARAMETER);

      CORBA::String_var ior =
        orb->object_to_string (test.in ()
                               ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG,
                  "\nConnecting to: %s\n\n",
                  ior.in ()));

      ACE_Profile_Timer timer;
      ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

      // We start an ACE_Profile_Timer here...
      timer.start ();

      int i = 0;

      for (i = 0; i < iterations; i++)
        {
          if (oneway && timed_method)
            {
              test->timed_oneway_method (timeout
                                         ACE_ENV_ARG_PARAMETER);
            }
          else if (oneway)
            {
              test->oneway_method ();
            }
          else if (!oneway && timed_method)
            {
              test->timed_method (timeout
                                  ACE_ENV_ARG_PARAMETER);
            }
          else
            {
              test->method ();
            }
        }

      // stop the timer.
      timer.stop ();
      timer.elapsed_time (elapsed_time);

      // compute average time.
      print_stats (elapsed_time, i);

      if (shutdown_server)
        {
          test->shutdown ();
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error!");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
