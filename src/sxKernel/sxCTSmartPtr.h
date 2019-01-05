//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer implementation. This very simple policy based design allows to use this template 
// implementation with various owenrships rules: Windows COM, intrusive counters, ...
template <typename t_Object, template<typename> class t_Policy>
class sxCTSmartPtr : private t_Policy<t_Object>
{
public:

	// Internal policy type
	typedef t_Policy<t_Object> _sxCPolicy;

	// Internal smart pointer type
	typedef sxCTSmartPtr<t_Object, t_Policy> _sxCTSmartPtr;

	// Internal pointee type
	typedef t_Object* sxCPointee;

	//-------------------------------------------------------------------------------------------------------
	// Default constructor
	sxCTSmartPtr(t_Object* a_pObject = NULL) :
		m_pObject(NULL)
	{
		Assign(a_pObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Construct from a reference to an object
	sxCTSmartPtr(t_Object& a_rObject) :
		m_pObject(NULL)
	{
		Assign(&a_rObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor.
	sxCTSmartPtr(_sxCTSmartPtr const& a_rSmartPtr) :
		m_pObject(NULL)
	{
		Assign(a_rSmartPtr.m_pObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor.
	template <typename t_OtherObject, template<typename> class t_OtherPolicy>
	sxCTSmartPtr(sxCTSmartPtr<t_OtherObject, t_OtherPolicy> const& a_rSmartPtr) :
		m_pObject(NULL)
	{
		Assign(a_rSmartPtr.m_pObject);
	}


	//-------------------------------------------------------------------------------------------------------
	// Destructor. Release ownership of the owned object.
	~sxCTSmartPtr()
	{
		Assign((t_Object*)NULL);
	}

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator
	void operator = (_sxCTSmartPtr const& a_rSmartPtr)
	{
		Assign(a_rSmartPtr.m_pObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator
	template <typename t_OtherObject, template<typename> class t_OtherPolicy>
	void operator = (sxCTSmartPtr<t_OtherObject, t_OtherPolicy> const& a_rSmartPtr)
	{
		Assign(a_rSmartPtr.m_pObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator
	void operator = (t_Object* a_pObject)
	{
		Assign(a_pObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator
	void operator = (t_Object& a_rObject)
	{
		Assign(&a_rObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// t_Object* cast operator
	operator t_Object* () const
	{
		return m_pObject;
	}

	//-------------------------------------------------------------------------------------------------------
	// t_Object& cast operator
	operator t_Object& () const
	{
		return sxDereference(m_pObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// t_Object& dereferencing operator (*)
	t_Object& operator *() const
	{
		return sxDereference(m_pObject);
	}

	//-------------------------------------------------------------------------------------------------------
	// t_Object* dereferencing operator (->)
	t_Object* operator ->() const
	{
		sxAssert(m_pObject != NULL, "Dereferencing NULL pointer");
		return m_pObject;
	}

	//-------------------------------------------------------------------------------------------------------
	// Comparison operator == 
	template <typename t_OtherObject, template<typename> class t_OtherPolicy>
	sxBool operator == (sxCTSmartPtr<t_OtherObject, t_OtherPolicy> const& a_rSmartPtr) const
	{
		return m_pObject == a_rSmartPtr.m_pObject;
	}

	//-------------------------------------------------------------------------------------------------------
	// Comparison operator !=
	template <typename t_OtherObject, template<typename> class t_OtherPolicy>
	sxBool operator != (sxCTSmartPtr<t_OtherObject, t_OtherPolicy> const& a_rSmartPtr) const
	{
		return m_pObject != a_rSmartPtr.m_pObject;
	}

private:

	//-------------------------------------------------------------------------------------------------------
	// Internal assignment function used by assignement operators
	template <typename t_OtherObject>
	void Assign(t_OtherObject* a_pObject)
	{
		// Early out if pointee doesn't change
		if(m_pObject == a_pObject)
		{
			return;
		}

		// Release previous object
		if(NULL != m_pObject)
		{
			_sxCPolicy::OnRelease(*m_pObject);
		}

		// Copy the new one
		m_pObject = a_pObject;

		// Acquire the new object
		if(NULL != m_pObject)
		{
			_sxCPolicy::OnAcquire(*m_pObject);
		}
	}

	// sxCTSmartPtr for any other type is friend to allow copies
	template <typename t_OtherObject, template<typename> class t_OtherPolicy>
	friend class sxCTSmartPtr;

	// Owned object
	sxCPointee m_pObject;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer default empty policy. Does nothing but letting sxCTSmartPtr doing its assertions on dereferencing
template <typename t_Object>
class keCTAssertPolicy
{
protected:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	keCTAssertPolicy() {}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	~keCTAssertPolicy() {}

	//-------------------------------------------------------------------------------------------------------
	// Acquire function called each time a new object is referenced
	void OnAcquire(t_Object& a_rObject) {}

	//-------------------------------------------------------------------------------------------------------
	// Release function called each time object ownership is released
	void OnRelease(t_Object& a_rObject) {}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer asserted type.
template <typename t_Object>
class sxCTAssertPtr
{
public:
	typedef sxCTSmartPtr<t_Object, keCTAssertPolicy> sxCType;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer COM policy.
template <typename t_Object>
class sxCTComPolicy
{
protected:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCTComPolicy() {}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	~sxCTComPolicy() {}

	//-------------------------------------------------------------------------------------------------------
	// Acquire function called each time a new object is referenced
	void OnAcquire(t_Object& a_rObject)
	{
		a_rObject.AddRef();
	}

	//-------------------------------------------------------------------------------------------------------
	// Release function called each time object ownership is released
	void OnRelease(t_Object& a_rObject)
	{
		a_rObject.Release();
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer COM type.
template <typename t_Object>
class sxCTComPtr
{
public:
	typedef sxCTSmartPtr<t_Object, sxCTComPolicy> sxCType;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer intrusive reference counter object.
class sxIIntrusiveCounter
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Get reference count.
	sxUInt GetRefCount()
	{
		return m_oiRefCount;
	}

protected:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize count to 0 by default
	sxIIntrusiveCounter(sxInt a_iInitialCount = 0) :
		m_oiRefCount(a_iInitialCount)
	{
		sxAssert(a_iInitialCount >= 0, "Count should never be negative");
	}

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize new count to 0
	sxIIntrusiveCounter(sxIIntrusiveCounter& a_rCounter):
		m_oiRefCount(0)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor.
	~sxIIntrusiveCounter()
	{
	}

	// sxCTIntrusivePolicy is friend in order to call AddRef/DecRef functions
	template<typename>
	friend class sxCTIntrusivePolicy;

	//-------------------------------------------------------------------------------------------------------
	// Increment reference count. Returns the new ref value
	sxUInt AddRef() const
	{
		sxUInt uiRes = ++m_oiRefCount;
		sxAssert(uiRes > 0, "Count should never be negative");
		return uiRes;
	}

	//-------------------------------------------------------------------------------------------------------
	// Decrement reference count. Returns the new ref value
	sxUInt DecRef() const
	{
		sxUInt uiRes = -- m_oiRefCount;
		sxAssert(uiRes >= 0, "Count should never be negative");
		return uiRes;
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy. Initialize new count to 0
	void operator = (sxIIntrusiveCounter& a_rCounter)
	{
		m_oiRefCount = 0;
	}

private:

	// Internal counter
	mutable sxCAtomicInt m_oiRefCount;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer intrusive reference counted policy.
template <typename t_Object>
class sxCTIntrusivePolicy
{
protected:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCTIntrusivePolicy() {}

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	~sxCTIntrusivePolicy() {}

	//-------------------------------------------------------------------------------------------------------
	// Acquire function called each time a new object is referenced
	void OnAcquire(t_Object& a_rObject)
	{
		a_rObject.AddRef();
	}

	//-------------------------------------------------------------------------------------------------------
	// Release function called each time object ownership is released
	void OnRelease(t_Object& a_rObject)
	{
		// Delete object if the last reference has been released
		if(a_rObject.DecRef() == 0)
		{
			sxDelete(a_rObject);
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer intrusive reference counted type.
template <typename t_Object>
class sxCTIntrusivePtr
{
public:
	typedef sxCTSmartPtr<t_Object, sxCTIntrusivePolicy> sxCType;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Smart pointer dereferencing
template<typename t_Object, template<typename> class t_Policy>
t_Object& sxDereference(sxCTSmartPtr<t_Object, t_Policy> const& a_rObject)
{
	sxAssert(a_rObject != NULL, "Dereferencing a NULL pointer");
	return *a_rObject;
}
