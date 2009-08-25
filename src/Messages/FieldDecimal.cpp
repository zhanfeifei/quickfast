// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldDecimal.h"
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

FieldDecimal::FieldDecimal()
  : Field(Field::DECIMAL, false)
{
}

FieldDecimal::FieldDecimal(const Decimal & value)
  : Field(Field::DECIMAL, true)
{
  signedInteger_ = value.getMantissa();
  exponent_ = value.getExponent();
}

FieldDecimal::FieldDecimal(mantissa_t mantissa, exponent_t exponent)
  : Field(Field::DECIMAL, true)
{
  signedInteger_ = mantissa;
  exponent_ = exponent;
}

FieldDecimal::~FieldDecimal()
{
}

const Decimal
FieldDecimal::toDecimal() const
{
  if(!valid_)
  {
    FieldNotPresent ex("Field not present");
  }
  return Decimal(signedInteger_, exponent_);
}

FieldCPtr
FieldDecimal::create(const Decimal & value)
{
  return new FieldDecimal(value);
}

FieldCPtr
FieldDecimal::create(mantissa_t mantissa, exponent_t exponent)
{
  return new FieldDecimal(mantissa, exponent);
}

FieldCPtr
FieldDecimal::createNull()
{
  return new FieldDecimal;
}
