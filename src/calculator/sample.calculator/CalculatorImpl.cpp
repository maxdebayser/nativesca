/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 *   
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/* $Rev: 489263 $ $Date: 2006-12-21 05:21:32 +0000 (Thu, 21 Dec 2006) $ */

#include <stdio.h>
#include <exception>

#include "ComponentContext.h"

using namespace puc::sca;

#include "CalculatorImpl.h"
#include "Divide.h"

CalculatorImpl::CalculatorImpl()
{
}
    
CalculatorImpl::~CalculatorImpl()
{
}

// Calculator interface
float CalculatorImpl::add(float arg1, float arg2)
{
    float result = arg1 + arg2;

    printf("CalculatorImpl::add %f + %f = %f\n", arg1, arg2, result);
    return result;
}

float CalculatorImpl::sub(float arg1, float arg2)
{
    float result = arg1 - arg2;
    printf("CalculatorImpl::sub %f - %f = %f\n", arg1, arg2, result);
    return result;
}

float CalculatorImpl::mul(float arg1, float arg2)
{
    float result = arg1 * arg2;
    printf("CalculatorImpl::mul %f * %f = %f\n", arg1, arg2, result);
    return result;
}

float CalculatorImpl::div(float arg1, float arg2)
{
    float result = 0;

    // This method shows how to invoke a service on a different component from within a component

    // First, get the current ComponentContext
	ComponentContext myContext = ComponentContext::getCurrent();

	try {
        // Find the required service, as referenced in CalculatorImpl.componentType
		VariantValue divideServiceV = myContext.getService("divideService");
		Divide* divideService = divideServiceV.convertToThrow<Divide*>();

        // Finally, invoke the service
        result = divideService->divide(arg1, arg2);

        printf("CalculatorImpl::div Divide returned result: %f\n", result);

	} catch (const std::exception& e) {
        // Print out error message and carry on
		printf("CalculatorImpl::div Error whilst invoking Divide: %s", e.what());
    }

    return result;
}
	

