//-----------------------------------------------------------------------------
// class ID templates

template<typename F> mxClassID float_type();
template<>           mxClassID float_type<float>()  { return mxSINGLE_CLASS; }
template<>           mxClassID float_type<double>() { return mxDOUBLE_CLASS; }

template<typename F> mxClassID int_type();
template<>           mxClassID int_type<int>()      { return mxINT32_CLASS; }
template<>           mxClassID int_type<unsigned>() { return mxUINT32_CLASS; }
