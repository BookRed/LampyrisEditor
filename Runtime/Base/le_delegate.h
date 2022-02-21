#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Base
 * \File:    le_delegate.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/

#ifndef LE_DELEGATE_H
#define LE_DELEGATE_H

/* usage of delegate && event

int function(int a, int b) {
	std::cout << a + b << std::endl;
	return 0;
}

class TestClass {
private:
	int m_a = 100;
public:
	int method(int a, int b) {
		std::cout << this->m_a + a + b << std::endl;
		return 0;
	}
};

int main() {
	LE_DELEGATE_DECLARE(SomeEvent, int(int, int));
	TestClass object;
	SomeEvent someEvent;
	someEvent += LE_EVENT_FUNCTION(MouseMoveEvent, function);
	someEvent += LE_EVENT_METHOD(MouseMoveEvent, TestClass, method, &object);
	mouseMoveEvent(2, 3);
}

// expected output:
// 5
// 7
// 107

*/

#define LE_DELEGATE_DECLARE(event_name,event_signature) \
	typedef LEDelegateEvent<event_signature> event_name;

#define LE_EVENT_FUNCTION(event_name,function_name) \
	event_name::EventHandler::create<&function_name>();

#define LE_EVENT_METHOD(event_name,class_name,method_name,class_object) \
	event_name::EventHandler::create<class_name,&class_name::method_name>(class_object);

template<typename Signature>
class LEDelegate;

template<typename Signature>
class LEDelegateEvent;

template<typename ReturnType, typename ...Args>
class LEDelegate<ReturnType(Args...)> {
public:
	typedef LEDelegate<ReturnType(Args...)> DelegateType;
private:
	typedef ReturnType(*Callable)(void* callss_object, Args...);
	void*    m_class_object;
	Callable m_callable;

public:
	ReturnType operator()(Args... args) const {
		return (*m_callable)(this->m_class_object, args...);
	}

	template<class T, ReturnType(T::* callable)(Args...)>
	static ReturnType methodWrapper(void* class_object, Args... args) {
		return (static_cast<T*>(class_object)->*callable)(args...);
	}

	template<ReturnType(*callable)(Args...)>
	static ReturnType functionWrapper(void* class_object, Args... args) {
		return (callable)(args...);
	}

	template<class T, ReturnType(T::* callable)(Args...)>
	static LEDelegate create(T* class_object) {
		LEDelegate delegate;
		delegate.m_class_object = static_cast<void*>(class_object);
		delegate.m_callable = &LEDelegate::methodWrapper<T, callable>;
		return delegate;
	}

	template<ReturnType(*callable)(Args...)>
	static LEDelegate create() {
		LEDelegate delegate;
		delegate.m_callable = &LEDelegate::functionWrapper<callable>;
		delegate.m_class_object = nullptr;
		return delegate;
	}
	operator bool() const {
		return this->m_callable != nullptr;
	}

	bool operator !() const {
		return this->m_callable == nullptr;
	}

	bool equal(const DelegateType& rhs) const {
		if (this->m_callable != rhs.m_callable) {
			return false;
		}
		if (this->m_class_object != rhs.m_class_object) {
			return false;
		}
		return true;
	}
};

template<typename ReturnType, typename ...Args>
class LEDelegateEvent<ReturnType(Args...)> {
public:
	typedef LEDelegate<ReturnType(Args...)> EventHandler;
	std::vector<EventHandler> m_handler;
public:
	int indexAt(const EventHandler& handler) const {
		for (int i = 0; i < this->m_handler.size(); i++) {
			if (this->m_handler[i].equal(handler)) {
				return i;
			}
		}
		return -1;
	}

	void operator()(Args... args) {
		for (int i = 0; i < this->m_handler.size(); i++) {
			this->m_handler[i](args...);
		}
	}

	void operator+=(const EventHandler& handler) {
		if (this->indexAt(handler) == -1) {
			this->m_handler.push_back(handler);
		}
	}

	void operator-=(const EventHandler& handler) {
		int index = this->indexAt(handler);
		if (index >= 0) {
			this->m_handler.erase(this->m_handler.begin() + index);
		}
	}

	void clear() {
		this->m_handler.clear();
	}
};

#endif // !LE_DELEGATE_H