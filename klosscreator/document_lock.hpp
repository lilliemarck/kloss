#pragma once

#include <boost/noncopyable.hpp>

struct Document;

namespace kloss {
namespace creator {

class document_lock : boost::noncopyable
{
public:
    document_lock(Document* document);
    ~document_lock();

    void lock() noexcept;
    void unlock() noexcept;

private:
    Document* document_;
    bool locked_;
};

} // namespace creator
} // namespace kloss
