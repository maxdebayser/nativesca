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

/* $Rev: 452753 $ $Date: 2006-10-04 07:19:15 +0100 (Wed, 04 Oct 2006) $ */

#ifndef sample_calculatorimpl_h
#define sample_calculatorimpl_h

#include "Calculator.h"
#include "Divide.h"

class CalculatorImpl : public Calculator
{
public:
    CalculatorImpl();
    virtual ~CalculatorImpl();

    // Calculator interface
	virtual float add(float arg1, float arg2);
	virtual float sub(float arg1, float arg2);
	virtual float mul(float arg1, float arg2);
	virtual float div(float arg1, float arg2);
	virtual float circleArea(float radius);

	void setPi(float p) { pi = p; }

	void setDivideService(Divide* d) { divideService = d; }

private:
	Divide* divideService;
	float pi = 3.14;
};

#endif // sample_calculatorimpl_h

