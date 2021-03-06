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

#include <iostream>
#include <stdlib.h>


#include "CompositeContext.h"
#include "Calculator.h"
#include <exception>
#include <tuscany/sca/core/Exceptions.h>

using namespace std;
using namespace puc::sca;


void usage();
bool IsNumber(const char *p);

int main(int argc, char* argv[])
{
    const char *operation;
    float arg1 = 0;
    float arg2 = 0;
	
    if (argc == 4)
    {
        operation = argv[1];
		
        if (!IsNumber(argv[2]))
        {
            cout << "calculator_client: Argument 1 is not a number" << endl;
			usage();
        }
        else
        {
            arg1 = atof(argv[2]);
        }

        if (!IsNumber(argv[3]))
        {
            cout << "calculator_client: Argument 2 is not a number" << endl;
			usage();
        }
        else
        {
            arg2 = atof(argv[3]);
        }
    }
    else
    {
        usage();
    }

    try
    {
        // Locate a service
		CompositeContext myContext = CompositeContext::getCurrent();

		VariantValue calcServiceV = myContext.locateService("CalculatorComponent/CalculatorService");
		Calculator *calcService = calcServiceV.convertToThrow<Calculator*>();

		if (calcService == 0)
		{
			cout << "calculator_client: Unable to find Calculator service" << endl;
		}
		else
		{
            float result = 0;
            if (strcmp(operation, "add") == 0)
            {
                result = calcService->add(arg1, arg2);
                cout << "calculator_client: add(" << arg1 << "," << arg2 << ") = " << result << endl;
            }
            else
            if (strcmp(operation, "sub") == 0)
            {
                result = calcService->sub(arg1, arg2);
                cout << "calculator_client: sub(" << arg1 << "," << arg2 << ") = " << result << endl;
            }
            else
            if (strcmp(operation, "mul") == 0)
            {
                result = calcService->mul(arg1, arg2);
                cout << "calculator_client: mul(" << arg1 << "," << arg2 << ") = " << result << endl;
            }
            else
            if (strcmp(operation, "div") == 0)
            {
                result = calcService->div(arg1, arg2);
                cout << "calculator_client: div(" << arg1 << "," << arg2 << ") = " << result << endl;
            }
			else
			if (strcmp(operation, "div") == 0)
			{
				result = calcService->div(arg1, arg2);
				cout << "calculator_client: div(" << arg1 << "," << arg2 << ") = " << result << endl;
			}
			else
		    {
		        cout << "calculator_client: Unrecognized operation: " << operation << endl;
			}
		}
		float area = calcService->circleArea(1);
		cout << "calculator_client: area of circle with radius 1: " << area << endl;
	}
	catch (const std::exception& ex)
	{
		cout << "calculator_client: exception caught: " << ex.what() << endl;
	}
	catch (const tuscany::sca::TuscanyRuntimeException& ex)
	{
		cout << "calculator_client: tuscany exception caught: " << ex.getMessageText() << endl;
	}

	return 0;
}

void usage()
{
   cout << "Usage: calculator_client add|sub|mul|div arg1 arg2" << endl;
   exit(1);
}

bool IsNumber (const char *p)
{
    int len = strlen(p);
    int pointcount = 0;

    if (!isdigit (p[0]) && p[0] != '-' && p[0] != '+')
    {
        return false;
    }
    for (int i = 1; i < len; i++)
    {
	   if (!isdigit (p[i]))
       {
           if (p[i] == '.')
           {
               if (pointcount > 0) return false;
               pointcount++;
           }
           else
           {
                return false;
           }
       }
    }
    return true;
}
