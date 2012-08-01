#pragma once

#include <boost/noncopyable.hpp>

namespace kloss {
namespace creator {

class document;

class document_lock : boost::noncopyable
{
public:
    document_lock(document& document);
    ~document_lock();

    void lock() noexcept;
    void unlock() noexcept;

private:
    document& document_;
    bool locked_;
};

} // namespace creator
} // namespace kloss
