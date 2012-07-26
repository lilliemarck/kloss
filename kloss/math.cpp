#include "math.hpp"

namespace kloss {

boost::optional<cml::vector3f> project(cml::vector3f const& point,
                                       cml::matrix44f_c const& model,
                                       cml::matrix44f_c const& projection,
                                       viewport const& viewport)
{
    cml::vector4f point4 = {point[0], point[1], point[2], 1.0f};
    cml::vector4f out = projection * model * point4;

    if (out[3] == 0.0f)
    {
        return {};
    }

    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];

    out[0] = out[0] * 0.5 + 0.5f;
    out[1] = out[1] * 0.5 + 0.5f;
    out[0] = out[0] * viewport.width + viewport.x;
    out[1] = out[1] * viewport.height + viewport.y;

    return boost::optional<cml::vector3f>({out[0], out[1], out[2]});
}

} // namespace kloss
