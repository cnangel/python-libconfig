#include <boost/python.hpp>
#include <libconfig.h++>
#include <string>

using namespace boost::python;
using namespace libconfig;

// ============================================================================
// pySetting — wrapper for libconfig::Setting
// ============================================================================

class pySetting
{
public:
    pySetting(Setting &setting) : setting(&setting) {}

    int getType() const { return setting->getType(); }
    const char *getName() const { return setting->getName(); }
    std::string getPath() const { return setting->getPath(); }
    int getIndex() const { return setting->getIndex(); }

    pySetting getParent() { return pySetting(setting->getParent()); }
    bool isRoot() const { return setting->isRoot(); }
    bool isGroup() const { return setting->isGroup(); }
    bool isArray() const { return setting->isArray(); }
    bool isList() const { return setting->isList(); }
    bool isAggregate() const { return setting->isAggregate(); }
    bool isScalar() const { return setting->isScalar(); }
    bool isNumber() const { return setting->isNumber(); }
bool isString() const { return setting->getType() == libconfig::Setting::TypeString; }
    int getLength() const { return setting->getLength(); }

#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 4)
    const char *getSourceFile() const { return setting->getSourceFile(); }
#endif
    unsigned int getSourceLine() const { return setting->getSourceLine(); }

#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 5)
    int getFormat() const { return setting->getFormat(); }
    void setFormat(int format) { setting->setFormat((Setting::Format)format); }
#endif

#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 5)
    pySetting lookup(const char *path) const
    {
        return pySetting(setting->lookup(path));
    }

    pySetting lookup(const std::string &path) const
    {
        return pySetting(setting->lookup(path.c_str()));
    }
#endif

    bool exists(const char *name) const { return setting->exists(name); }

    pySetting add(const char *name, int type)
    {
        return pySetting(setting->add(name, (Setting::Type)type));
    }
    pySetting add(int type)
    {
        return pySetting(setting->add((Setting::Type)type));
    }

    void remove(const char *name) { setting->remove(name); }
    void removeAt(unsigned int idx) { setting->remove(idx); }

    // __getitem__ for index access
    pySetting getChildByIndex(int index) const
    {
        return pySetting((*setting)[index]);
    }

    pySetting getChildByName(const char *name) const
    {
        return pySetting((*setting)[name]);
    }

    // setValue — type dispatch based on Setting type
    void setValue(const object &val)
    {
        switch (setting->getType()) {
        case Setting::TypeInt:
            if (PyLong_Check(val.ptr()))
                *setting = extract<int>(val)();
            else if (PyBool_Check(val.ptr()))
                *setting = extract<int>(val)();
            else
                throw std::runtime_error("setValue: expected int for TypeInt setting");
            break;
        case Setting::TypeInt64:
            if (PyLong_Check(val.ptr()))
                *setting = extract<long long>(val)();
            else if (PyBool_Check(val.ptr()))
                *setting = extract<long long>(val)();
            else
                throw std::runtime_error("setValue: expected int for TypeInt64 setting");
            break;
        case Setting::TypeFloat:
            if (PyFloat_Check(val.ptr()) || PyLong_Check(val.ptr()) || PyBool_Check(val.ptr()))
                *setting = extract<double>(val)();
            else
                throw std::runtime_error("setValue: expected number for TypeFloat setting");
            break;
        case Setting::TypeString:
            if (PyUnicode_Check(val.ptr()))
                *setting = extract<const char *>(val)();
            else if (PyBytes_Check(val.ptr()))
                *setting = extract<const char *>(val)();
            else {
                object s = val.attr("__str__")();
                *setting = extract<const char *>(s)();
            }
            break;
        case Setting::TypeBoolean:
            if (PyBool_Check(val.ptr()))
                *setting = (val.ptr() == Py_True);
            else
                *setting = extract<bool>(val)();
            break;
        default:
            throw std::runtime_error("setValue: cannot set value on aggregate type");
        }
    }

    // getValue — automatic type detection, returns Python object
    object getValue() const
    {
        switch (setting->getType()) {
            case Setting::TypeInt:
                return object((int)*setting);
            case Setting::TypeInt64:
                return object((long long)*setting);
            case Setting::TypeFloat:
                return object((double)*setting);
            case Setting::TypeString:
                return object((const char *)*setting);
            case Setting::TypeBoolean:
                return object((bool)*setting);
            default:
                return object();
        }
    }

    // Iterator support
    list children()
    {
        list result;
        int length = setting->getLength();
        for (int i = 0; i < length; i++)
            result.append(pySetting((*setting)[i]));
        return result;
    }

private:
    Setting *setting;
};

// ============================================================================
// pyConfig — C++ wrapper for libconfig::Config
// ============================================================================

class pyConfig
{
public:
    pyConfig()
    {
        config = new Config();
    }

    ~pyConfig()
    {
        delete config;
    }

    void read(FILE *stream)
    { config->read(stream); }

    void write(FILE *stream)
    { config->write(stream); }

    void readFile(const char *filename)
    { config->readFile(filename); }

    void writeFile(const char *filename)
    { config->writeFile(filename); }

#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 4)
    void readString(const char *stringdata)
    { config->readString(stringdata); }

    void readString(const std::string &stringdata)
    { config->readString(stringdata.c_str()); }
#endif

    bool getAutoConvert()
    { return config->getAutoConvert(); }

    void setAutoConvert(bool flag)
    { config->setAutoConvert(flag); }

    bool exists(const char *path)
    { return config->exists(path); }

    // ========== libconfig 1.8+ options ==========
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 8)
    void setOptions(int options)
    { config->setOptions(options); }

    int getOptions() const
    { return config->getOptions(); }

    void setOption(int option, bool flag)
    { config->setOption((Config::Option)option, flag); }

    bool getOption(int option) const
    { return config->getOption((Config::Option)option); }

    void setFloatPrecision(unsigned short digits)
    { config->setFloatPrecision(digits); }

    unsigned short getFloatPrecision() const
    { return config->getFloatPrecision(); }

    void setTabWidth(unsigned short width)
    { config->setTabWidth(width); }

    unsigned short getTabWidth() const
    { return config->getTabWidth(); }

    void setDefaultFormat(int format)
    { config->setDefaultFormat((Setting::Format)format); }

    int getDefaultFormat() const
    { return config->getDefaultFormat(); }

    void clear()
    { config->clear(); }
#endif

    // ========== Root / lookup ==========

    pySetting getRoot()
    { return pySetting(config->getRoot()); }

    pySetting lookup(const char *path)
    { return pySetting(config->lookup(path)); }

    // ========== value() — ordered type probing ==========

    tuple value(const char *path)
    {
        std::string v_string;
        if (config->lookupValue(path, v_string))
            return make_tuple(v_string.c_str(), true);

        int v_int;
        if (config->lookupValue(path, v_int))
            return make_tuple(v_int, true);

        unsigned int v_uint;
        if (config->lookupValue(path, v_uint))
            return make_tuple(v_uint, true);

        bool v_bool;
        if (config->lookupValue(path, v_bool))
            return make_tuple(v_bool, true);

        unsigned long long v_ulonglong;
        if (config->lookupValue(path, v_ulonglong))
            return make_tuple(v_ulonglong, true);

        long long v_longlong;
        if (config->lookupValue(path, v_longlong))
            return make_tuple(v_longlong, true);

        float v_float;
        if (config->lookupValue(path, v_float))
            return make_tuple(v_float, true);

        double v_double;
        if (config->lookupValue(path, v_double))
            return make_tuple(v_double, true);

        return make_tuple("", false);
    }

    // ========== Type-safe lookup methods ==========

    int lookupInt(const char *path, int default_ = 0)
    {
        int v;
        return config->lookupValue(path, v) ? v : default_;
    }

    long long lookupInt64(const char *path, long long default_ = 0)
    {
        long long v;
        return config->lookupValue(path, v) ? v : default_;
    }

    double lookupFloat(const char *path, double default_ = 0.0)
    {
        double v;
        return config->lookupValue(path, v) ? v : default_;
    }

    bool lookupBool(const char *path, bool default_ = false)
    {
        bool v;
        return config->lookupValue(path, v) ? v : default_;
    }

    std::string lookupString(const char *path, const char *default_ = "")
    {
        std::string v;
        return config->lookupValue(path, v) ? v : std::string(default_);
    }

    // ========== readList — backward-compat: returns list of (value, found) tuples ==========

    list readList(const char *path)
    {
        list result;
        try {
            Setting &s = config->lookup(path);
            int length = s.getLength();
            for (int i = 0; i < length; i++)
                result.append(value(s[i].getPath().c_str()));
        }
        catch (SettingNotFoundException &) {}
        return result;
    }

    // ========== children ==========

    list children_root()
    {
        list result;
        Setting &root = config->getRoot();
        int length = root.getLength();
        for (int i = 0; i < length; i++)
            result.append(root[i].getPath().c_str());
        return result;
    }

    list children(const char *path)
    {
        list result;
        try {
            Setting &s = (path && *path) ? config->lookup(path) : config->getRoot();
            int length = s.getLength();
            for (int i = 0; i < length; i++)
                result.append(s[i].getPath().c_str());
        }
        catch (SettingNotFoundException &) {}
        return result;
    }

    // ========== remove ==========

    void remove(const char *path, const char *name)
    { config->lookup(path).remove(name); }

    // ========== add* methods — support empty path ==========

    static Setting &resolveParent(Config *cfg, const char *path)
    {
        return (path && *path) ? cfg->lookup(path) : cfg->getRoot();
    }

    void addBoolean(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeBoolean); }

    void addBigInteger(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeInt64); }

    void addInteger(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeInt); }

    void addFloat(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeFloat); }

    void addString(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeString); }

    void addGroup(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeGroup); }

    void addList(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeList); }

    void addArray(const char *path, const char *name)
    { resolveParent(config, path).add(name, Setting::TypeArray); }

    // ========== appendToList ==========

    void appendToList(const char *path, const char *value)
    { config->lookup(path).add(Setting::TypeString) = value; }

    // ========== setValue — type dispatch based on Setting type ==========

    void setValue(const char *path, const object &val)
    {
        Setting &s = config->lookup(path);
        switch (s.getType()) {
        case Setting::TypeInt:
            if (PyLong_Check(val.ptr()))
                s = extract<int>(val)();
            else if (PyBool_Check(val.ptr()))
                s = extract<int>(val)();
            else
                throw std::runtime_error("setValue: expected int for TypeInt setting");
            break;
        case Setting::TypeInt64:
            if (PyLong_Check(val.ptr()))
                s = extract<long long>(val)();
            else if (PyBool_Check(val.ptr()))
                s = extract<long long>(val)();
            else
                throw std::runtime_error("setValue: expected int for TypeInt64 setting");
            break;
        case Setting::TypeFloat:
            if (PyFloat_Check(val.ptr()) || PyLong_Check(val.ptr()) || PyBool_Check(val.ptr()))
                s = extract<double>(val)();
            else
                throw std::runtime_error("setValue: expected number for TypeFloat setting");
            break;
        case Setting::TypeString:
            if (PyUnicode_Check(val.ptr()))
                s = extract<const char *>(val)();
            else if (PyBytes_Check(val.ptr()))
                s = extract<const char *>(val)();
            else {
                object str_val = val.attr("__str__")();
                s = extract<const char *>(str_val)();
            }
            break;
        case Setting::TypeBoolean:
            if (PyBool_Check(val.ptr()))
                s = (val.ptr() == Py_True);
            else
                s = extract<bool>(val)();
            break;
        default:
            throw std::runtime_error("setValue: cannot set value on aggregate type");
        }
    }

private:
    Config *config;
};

// ============================================================================
// BOOST_PYTHON_MODULE
// ============================================================================

BOOST_PYTHON_MODULE(pylibconfig)
{
    // ---- Constants ----

    // Setting::Type
    enum_<Setting::Type>("SettingType")
        .value("TYPE_NONE",    Setting::TypeNone)
        .value("TYPE_INT",     Setting::TypeInt)
        .value("TYPE_INT64",   Setting::TypeInt64)
        .value("TYPE_FLOAT",   Setting::TypeFloat)
        .value("TYPE_STRING",  Setting::TypeString)
        .value("TYPE_BOOLEAN", Setting::TypeBoolean)
        .value("TYPE_GROUP",   Setting::TypeGroup)
        .value("TYPE_ARRAY",   Setting::TypeArray)
        .value("TYPE_LIST",    Setting::TypeList)
    ;

    // Export as module-level constants
    scope().attr("TYPE_NONE")    = (int)Setting::TypeNone;
    scope().attr("TYPE_INT")     = (int)Setting::TypeInt;
    scope().attr("TYPE_INT64")   = (int)Setting::TypeInt64;
    scope().attr("TYPE_FLOAT")   = (int)Setting::TypeFloat;
    scope().attr("TYPE_STRING")  = (int)Setting::TypeString;
    scope().attr("TYPE_BOOLEAN") = (int)Setting::TypeBoolean;
    scope().attr("TYPE_GROUP")   = (int)Setting::TypeGroup;
    scope().attr("TYPE_ARRAY")   = (int)Setting::TypeArray;
    scope().attr("TYPE_LIST")    = (int)Setting::TypeList;

    // Setting::Format (libconfig 1.5+)
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 5)
    enum_<Setting::Format>("SettingFormat")
        .value("FORMAT_DEFAULT", Setting::FormatDefault)
        .value("FORMAT_HEX",     Setting::FormatHex)
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 8)
        .value("FORMAT_BIN",     Setting::FormatBin)
        .value("FORMAT_OCT",     Setting::FormatOct)
#endif
    ;
    scope().attr("FORMAT_DEFAULT") = (int)Setting::FormatDefault;
    scope().attr("FORMAT_HEX")     = (int)Setting::FormatHex;
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 8)
    scope().attr("FORMAT_BIN")     = (int)Setting::FormatBin;
    scope().attr("FORMAT_OCT")     = (int)Setting::FormatOct;
#endif
#endif

    // Config::Option (libconfig 1.8+)
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 8)
    enum_<Config::Option>("ConfigOption")
        .value("OPTION_AUTOCONVERT",                    Config::OptionAutoConvert)
        .value("OPTION_SEMICOLON_SEPARATORS",           Config::OptionSemicolonSeparators)
        .value("OPTION_COLON_ASSIGNMENT_FOR_GROUPS",    Config::OptionColonAssignmentForGroups)
        .value("OPTION_COLON_ASSIGNMENT_FOR_NON_GROUPS",Config::OptionColonAssignmentForNonGroups)
        .value("OPTION_OPEN_BRACE_ON_SEPARATE_LINE",    Config::OptionOpenBraceOnSeparateLine)
        .value("OPTION_ALLOW_SCIENTIFIC_NOTATION",      Config::OptionAllowScientificNotation)
        .value("OPTION_FSYNC",                          Config::OptionFsync)
        .value("OPTION_ALLOW_OVERRIDES",                Config::OptionAllowOverrides)
    ;
    scope().attr("OPTION_AUTOCONVERT")                     = (int)Config::OptionAutoConvert;
    scope().attr("OPTION_SEMICOLON_SEPARATORS")            = (int)Config::OptionSemicolonSeparators;
    scope().attr("OPTION_COLON_ASSIGNMENT_FOR_GROUPS")     = (int)Config::OptionColonAssignmentForGroups;
    scope().attr("OPTION_COLON_ASSIGNMENT_FOR_NON_GROUPS") = (int)Config::OptionColonAssignmentForNonGroups;
    scope().attr("OPTION_OPEN_BRACE_ON_SEPARATE_LINE")     = (int)Config::OptionOpenBraceOnSeparateLine;
    scope().attr("OPTION_ALLOW_SCIENTIFIC_NOTATION")       = (int)Config::OptionAllowScientificNotation;
    scope().attr("OPTION_FSYNC")                           = (int)Config::OptionFsync;
    scope().attr("OPTION_ALLOW_OVERRIDES")                 = (int)Config::OptionAllowOverrides;
#endif

    // ---- pySetting ----
    class_<pySetting>("Setting", no_init)
        .def("getType",       &pySetting::getType)
        .def("getName",       &pySetting::getName)
        .def("getPath",       &pySetting::getPath)
        .def("getIndex",      &pySetting::getIndex)
        .def("getParent",     &pySetting::getParent)
        .def("isRoot",        &pySetting::isRoot)
        .def("isGroup",       &pySetting::isGroup)
        .def("isArray",       &pySetting::isArray)
        .def("isList",        &pySetting::isList)
        .def("isAggregate",   &pySetting::isAggregate)
        .def("isScalar",      &pySetting::isScalar)
        .def("isNumber",      &pySetting::isNumber)
.def("isString",      &pySetting::isString)
        .def("getLength",     &pySetting::getLength)
        .def("__len__",       &pySetting::getLength)
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 4)
        .def("getSourceFile", &pySetting::getSourceFile)
#endif
        .def("getSourceLine", &pySetting::getSourceLine)
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 5)
        .def("getFormat",     &pySetting::getFormat)
        .def("setFormat",     &pySetting::setFormat)
#endif
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 5)
        .def("lookup",        (pySetting (pySetting::*)(const char *) const) &pySetting::lookup)
        .def("lookup",        (pySetting (pySetting::*)(const std::string &) const) &pySetting::lookup)
#endif
        .def("exists",        &pySetting::exists)
        .def("add",           (pySetting (pySetting::*)(const char *, int)) &pySetting::add)
        .def("add",           (pySetting (pySetting::*)(int)) &pySetting::add)
        .def("remove",        (void (pySetting::*)(const char *)) &pySetting::remove)
        .def("removeAt",      &pySetting::removeAt)
        .def("__getitem__",   &pySetting::getChildByIndex)
        .def("__getitem__",   &pySetting::getChildByName)
        .def("children",      &pySetting::children)
        .def("setValue",      &pySetting::setValue)
        .def("getValue",      &pySetting::getValue)
    ;

    // ---- pyConfig ----
    class_<pyConfig>("Config")
        .def("read",          &pyConfig::read)
        .def("write",         &pyConfig::write)
        .def("readFile",      &pyConfig::readFile)
        .def("writeFile",     &pyConfig::writeFile)
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 4)
        .def("readString",    (void (pyConfig::*)(const char *)) &pyConfig::readString)
        .def("readString",    (void (pyConfig::*)(const std::string &)) &pyConfig::readString)
#endif
        .def("getAutoConvert",&pyConfig::getAutoConvert)
        .def("setAutoConvert",&pyConfig::setAutoConvert)
        .def("exists",        &pyConfig::exists)
        // options (1.8+)
#if LIBCONFIGXX_VER_MAJOR > 1 || (LIBCONFIGXX_VER_MAJOR == 1 && LIBCONFIGXX_VER_MINOR >= 8)
        .def("setOptions",        &pyConfig::setOptions)
        .def("getOptions",        &pyConfig::getOptions)
        .def("setOption",         &pyConfig::setOption)
        .def("getOption",         &pyConfig::getOption)
        .def("setFloatPrecision", &pyConfig::setFloatPrecision)
        .def("getFloatPrecision", &pyConfig::getFloatPrecision)
        .def("setTabWidth",       &pyConfig::setTabWidth)
        .def("getTabWidth",       &pyConfig::getTabWidth)
        .def("setDefaultFormat",  &pyConfig::setDefaultFormat)
        .def("getDefaultFormat",  &pyConfig::getDefaultFormat)
        .def("clear",             &pyConfig::clear)
#endif
        .def("getRoot",       &pyConfig::getRoot)
        .def("lookup",        &pyConfig::lookup)
        .def("lookupInt",     &pyConfig::lookupInt,
             (arg("path"), arg("default_") = 0))
        .def("lookupInt64",   &pyConfig::lookupInt64,
             (arg("path"), arg("default_") = 0))
        .def("lookupFloat",   &pyConfig::lookupFloat,
             (arg("path"), arg("default_") = 0.0))
        .def("lookupBool",    &pyConfig::lookupBool,
             (arg("path"), arg("default_") = false))
        .def("lookupString",  &pyConfig::lookupString,
             (arg("path"), arg("default_") = ""))
        .def("value",         &pyConfig::value)
        .def("readList",      &pyConfig::readList)
        .def("children",      &pyConfig::children)
        .def("children",      &pyConfig::children_root)
        .def("remove",        &pyConfig::remove)
        .def("addString",     &pyConfig::addString)
        .def("addBoolean",    &pyConfig::addBoolean)
        .def("addBigInteger", &pyConfig::addBigInteger)
        .def("addInteger",    &pyConfig::addInteger)
        .def("addFloat",      &pyConfig::addFloat)
        .def("addGroup",      &pyConfig::addGroup)
        .def("addList",       &pyConfig::addList)
        .def("addArray",      &pyConfig::addArray)
        .def("setValue",      &pyConfig::setValue)
        .def("appendToList",  &pyConfig::appendToList)
    ;
}