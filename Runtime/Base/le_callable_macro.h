#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_callable_macro.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_CALLABLE_MACRO_H
#define LE_CALLABLE_MACRO_H

// LE Includes
#include "le_callable.h"

// acquire the parameter offset among the parameter struct
#define LE_PARAM_ADDRESS(callable,parameter,param_index) \
	callable->getParameterAt(param_index)->getPropertyPointer(parameter) 	      
																			      
// parameter passing														      
#define LE_INVOKE_CALLABLE(class_name,class_object,callable_name,...) \
	((class_name *)class_object)->callable_name(##__VA_ARGS__);		       

// used for the declaration of macros, such as 'LE_CALLABLE_NONE_PARAM'...
#define LE_NONE_PARAMETER
#define LE_ONE_PARAMETER                      ParamType1,ParamName1
#define LE_TWO_PARAMETER   LE_ONE_PARAMETER,  ParamType2,ParamName2
#define LE_THREE_PARAMETER LE_TWO_PARAMETER,  ParamType3,ParamName3
#define LE_FOUR_PARAMETER  LE_THREE_PARAMETER,ParamType4,ParamName4
#define LE_FIVE_PARAMETER  LE_FOUR_PARAMETER, ParamType5,ParamName5
#define LE_SIX_PARAMETER   LE_FIVE_PARAMETER, ParamType6,ParamName6
#define LE_SEVEN_PARAMETER LE_SIX_PARAMETER,  ParamType7,ParamName7
#define LE_EIGHT_PARAMETER LE_SEVEN_PARAMETER,ParamType8,ParamName8

// used for 'LE_INVOKE_CALLABLE'
#define LE_NONE_PARAMETER_PASS  classname,object,callable
#define LE_ONE_PARAMETER_PASS   LE_ONE_PARAMETER_PASS,  ParamName1
#define LE_TWO_PARAMETER_PASS   LE_ONE_PARAMETER_PASS,  ParamName2
#define LE_THREE_PARAMETER_PASS LE_TWO_PARAMETER_PASS,  ParamName3
#define LE_FOUR_PARAMETER_PASS  LE_THREE_PARAMETER_PASS,ParamName4
#define LE_FIVE_PARAMETER_PASS  LE_FOUR_PARAMETER_PASS, ParamName5
#define LE_SIX_PARAMETER_PASS   LE_FIVE_PARAMETER_PASS, ParamName6
#define LE_SEVEN_PARAMETER_PASS LE_SIX_PARAMETER_PASS,  ParamName7
#define LE_EIGHT_PARAMETER_PASS LE_SEVEN_PARAMETER_PASS,ParamName8

// used for acquiring parameter from parameter pack
#define LE_UNPACK_ONE_PARAMETER   \
	ParamType1 ParamName1 = *(ParamType1*)(LE_PARAM_ADDRESS(callable,parameter,0));

#define LE_UNPACK_TWO_PARAMETER   \
	LE_UNPACK_ONE_PARAMETER;      \
	ParamType2 ParamName2 = *(ParamType2*)(LE_PARAM_ADDRESS(callable,parameter,1));

#define LE_UNPACK_THREE_PARAMETER \
	LE_UNPACK_TWO_PARAMETER;      \
	ParamType3 ParamName3 = *(ParamType3*)(LE_PARAM_ADDRESS(callable,parameter,2));

#define LE_UNPACK_FOUR_PARAMETER  \
	LE_UNPACK_THREE_PARAMETER;    \
	ParamType4 ParamName4 = *(ParamType4*)(LE_PARAM_ADDRESS(callable,parameter,3));

#define LE_UNPACK_FIVE_PARAMETER  \
	LE_UNPACK_FOUR_PARAMETER;     \
	ParamType5 ParamName5 = *(ParamType5*)(LE_PARAM_ADDRESS(callable,parameter,4));

#define LE_UNPACK_SIX_PARAMETER   \
	LE_UNPACK_FIVE_PARAMETER;     \
	ParamType6 ParamName6 = *(ParamType6*)(LE_PARAM_ADDRESS(callable,parameter,5));

#define LE_UNPACK_SEVEN_PARAMETER \
	LE_UNPACK_SIX_PARAMETER;      \
	ParamType7 ParamName7 = *(ParamType7*)(LE_PARAM_ADDRESS(callable,parameter,6));

#define LE_UNPACK_EIGHT_PARAMETER \
	LE_UNPACK_SEVEN_PARAMETER;    \
	ParamType8 ParamName8 = *(ParamType8*)(LE_PARAM_ADDRESS(callable,parameter,7));

#define LE_CALLABLE_NO_RETURN   class_name,callable_name
#define LE_CALLABLE_WITH_RETURN LE_CALLABLE_NO_RETURN,returnType

#define LE_INVOKE_CALLABLE_BODY \
	static void invokeCallable(LEObject   *object, 	    \
                               LECallable *callable, 	\
					           void       *parameter, 	\
					           void       *returnValue) \

#define LE_REQUIRED_RETURN \
	returnValue!=nullptr && callable->isNoReturnCallable()


#define LE_CALLABLE_NONE_PARAM(LE_CALLABLE_WITH_RETURN)                   \
class LECallableExecutor_##class_name##_##callable_name {                   \
public:                                                                   \
	LECallableExecutor_##class_name##_##callable_name() {}                  \
	LE_INVOKE_CALLABLE_BODY {                                             \
		if(LE_REQUIRED_RETURN) {                                          \
			(*(returnValue*))=LE_INVOKE_CALLABLE(LE_NONE_PARAMETER_PASS); \
		}                                                                 \
		else LE_INVOKE_CALLABLE(LE_NONE_PARAMETER_PASS);                  \
	}                                                                     \
};																			       
																			      
#define LE_CALLABLE_NONE_PARAM_NORETURN(LE_CALLABLE_NO_RETURN)            \
class LECallableExecutor_##class_name##_callable_name {                   \
public:                                                                   \
	LECallableExecutor_##class_name##_callable_name() {}                  \
	LE_INVOKE_CALLABLE_BODY {                                             \
		LE_INVOKE_CALLABLE(LE_NONE_PARAMETER_PASS);                       \
	}                                                                     \
};		

#define LE_CALLABLE_ONE_PARAM(LE_CALLABLE_WITH_RETURN,LE_ONE_PARAMETER)   \
class LECallableExecutor_##class_name##_callable_name {                   \
public:                                                                   \
	LECallableExecutor_##class_name##_callable_name() {}                  \
	LE_INVOKE_CALLABLE_BODY {                                             \
		LE_UNPACK_ONE_PARAMETER                                           \
		if(LE_REQUIRED_RETURN) {                                          \
			(*(returnValue*))=LE_INVOKE_CALLABLE(LE_ONE_PARAMETER_PASS);  \
		}                                                                 \
		else LE_INVOKE_CALLABLE(LE_ONE_PARAMETER_PASS);                   \
	}                                                                     \
};		

#define LE_CALLABLE_ONE_PARAM_NORETURN(LE_CALLABLE_NO_RETURN,LE_ONE_PARAMETER)  \
class LECallableExecutor_##class_name##_callable_name{                          \
public:                                                                         \
	LECallableExecutor_##class_name##_callable_name() {}                        \
	LE_INVOKE_CALLABLE_BODY {                                                   \
		LE_UNPACK_ONE_PARAMETER                                                 \
		LE_INVOKE_CALLABLE(LE_ONE_PARAMETER_PASS);                              \
	}                                                                           \
};		
/*
Macro Param1: class_name
Macro Param2: class_method_name
*/
#define LE_CALLABLE_TWO_PARAM(LE_CALLABLE_WITH_RETURN,LE_TWO_PARAMETER)     			

#define LE_CALLABLE_TWO_PARAM_NORETURN(LE_CALLABLE_NO_RETURN,LE_TWO_PARAMETER)

#define LE_CALLABLE_THREE_PARAM(LE_CALLABLE_WITH_RETURN,LE_THREE_PARAMETER)

#define LE_CALLABLE_THREE_PARAM_NORETURN(LE_CALLABLE_NO_RETURN,LE_THREE_PARAMETER)

#define LE_CALLABLE_FOUR_PARAM(LE_CALLABLE_WITH_RETURN,LE_FOUR_PARAMETER)

#define LE_CALLABLE_FOUR_PARAM_NORETURN(LE_CALLABLE_WITH_RETURN,LE_FOUR_PARAMETER)

#define LE_CALLABLE_FIVE_PARAM(LE_CALLABLE_WITH_RETURN,LE_FIVE_PARAMETER)

#define LE_CALLABLE_FIVE_PARAM_NORETURN(LE_CALLABLE_NO_RETURN,LE_FIVE_PARAMETER)

#define LE_CALLABLE_SIX_PARAM(LE_CALLABLE_WITH_RETURN,LE_SIX_PARAMETER)

#define LE_CALLABLE_SIX_PARAM_NORETURN(LE_CALLABLE_NO_RETURN,LE_SIX_PARAMETER)

#define LE_CALLABLE_SEVEN_PARAM(LE_CALLABLE_WITH_RETURN,LE_SEVEN_PARAMETER)

#define LE_CALLABLE_SEVEN_PARAM_NORETURN(LE_CALLABLE_NO_RETURN,LE_SEVEN_PARAMETER)

#define LE_CALLABLE_EIGHT_PARAM(LE_CALLABLE_WITH_RETURN,LE_EIGHT_PARAMETER)

#define LE_CALLABLE_EIGHT_PARAM_NORETURN(LE_CALLABLE_NO_RETURN,LE_EIGHT_PARAMETER)

#endif // !LE_CALLABLE_MACRO_H