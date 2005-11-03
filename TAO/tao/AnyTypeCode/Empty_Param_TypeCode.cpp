// $Id$

#include "Empty_Param_TypeCode.h"

#ifndef __ACE_INLINE__
# include "Empty_Param_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


ACE_RCSID (tao,
           Empty_Param_TypeCode,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Empty_Param::tao_marshal (TAO_OutputCDR &,
                                         CORBA::ULong) const
{
  // Empty parameter list.  Nothing to marshal.

  return true;
}

void
TAO::TypeCode::Empty_Param::tao_duplicate (void)
{
  // No-op since empty parameter TypeCodes are never created
  // dynamically, meaning there is no need to implement reference
  // counting.
}

void
TAO::TypeCode::Empty_Param::tao_release (void)
{
  // No-op since empty parameter TypeCodes are never created
  // dynamically, meaning there is no need to implement reference
  // counting.
}

CORBA::Boolean
TAO::TypeCode::Empty_Param::equal_i (CORBA::TypeCode_ptr
                                     ACE_ENV_ARG_DECL_NOT_USED) const
{
  // Equality has already been established in the
  // CORBA::TypeCode base class.

  return 1;
}

CORBA::Boolean
TAO::TypeCode::Empty_Param::equivalent_i (CORBA::TypeCode_ptr tc
                                          ACE_ENV_ARG_DECL) const
{
  // We could refactor this code to the CORBA::TypeCode::equivalent()
  // method but doing so would force us to determine the unaliased
  // kind of this TypeCode.  Since we already know the unaliased kind
  // of this TypeCode, choose to optimize away the additional kind
  // unaliasing operation rather than save space.

  CORBA::TCKind const tc_kind =
    TAO::unaliased_kind (tc
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind != this->kind_)
    return 0;

  return 1;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Empty_Param::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Already compact since parameter list is empty.

  // Since empty parameter TypeCodes are never created
  // dynamically, there is no need to manipulate a reference count.

  static TAO::TypeCode::Empty_Param compact_typecode (this->kind_);

  return &compact_typecode;
}

TAO_END_VERSIONED_NAMESPACE_DECL
