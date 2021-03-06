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

#include "DivideImpl.h"

DivideImpl::DivideImpl()
{
}
    
DivideImpl::~DivideImpl()
{
}

// Divide interface
float DivideImpl::divide(float arg1, float arg2)
{
    if(arg2 == 0.0)
    {
        printf("DivideImpl::div %f / %f !! Cannot divide by zero, so returning 0\n", arg1, arg2);
        return 0;
    }

    float result = arg1 / arg2;
    printf("DivideImpl::div %f / %f = %f\n", arg1, arg2, result);
    return result;
}
	

