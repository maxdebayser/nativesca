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

#ifndef sample_calculator_h
#define sample_calculator_h

class Calculator  
{
public:
	virtual ~Calculator() {}
	virtual float add(float arg1, float arg2) = 0;
	virtual float sub(float arg1, float arg2) = 0;
	virtual float mul(float arg1, float arg2) = 0;
	virtual float div(float arg1, float arg2) = 0;
	virtual float circleArea(float radius) = 0;
};

#endif // sample_calculator_h
