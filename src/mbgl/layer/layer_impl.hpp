#ifndef MBGL_STYLE_STYLE_LAYER
#define MBGL_STYLE_STYLE_LAYER

#include <mbgl/layer/layer.hpp>
#include <mbgl/style/types.hpp>
#include <mbgl/style/filter_expression.hpp>
#include <mbgl/renderer/render_pass.hpp>
#include <mbgl/util/noncopyable.hpp>

#include <rapidjson/document.h>

#include <memory>
#include <string>
#include <limits>

namespace mbgl {

class StyleCascadeParameters;
class StyleCalculationParameters;
class StyleBucketParameters;
class Bucket;

using JSVal = rapidjson::Value;

class Layer::Impl {
public:
    virtual ~Impl() = default;

    // Check whether this layer is of the given subtype.
    template <class T> bool is() const { return dynamic_cast<const T*>(this); }

    // Dynamically cast this layer to the given subtype.
    template <class T>       T* as()       { return dynamic_cast<      T*>(this); }
    template <class T> const T* as() const { return dynamic_cast<const T*>(this); }

    // Create a copy of this layer.
    virtual std::unique_ptr<Layer::Impl> clone() const = 0;

    virtual void parseLayout(const JSVal& value) = 0;
    virtual void parsePaints(const JSVal& value) = 0;

    // If the layer has a ref, the ref. Otherwise, the id.
    const std::string& bucketName() const;

    // Partially evaluate paint properties based on a set of classes.
    virtual void cascade(const StyleCascadeParameters&) = 0;

    // Fully evaluate cascaded paint properties based on a zoom level.
    // Returns true if any paint properties have active transitions.
    virtual bool recalculate(const StyleCalculationParameters&) = 0;

    virtual std::unique_ptr<Bucket> createBucket(StyleBucketParameters&) const = 0;

    // Checks whether this layer needs to be rendered in the given render pass.
    bool hasRenderPass(RenderPass) const;

public:
    std::string id;
    std::string ref;
    std::string source;
    std::string sourceLayer;
    FilterExpression filter;
    float minZoom = -std::numeric_limits<float>::infinity();
    float maxZoom = std::numeric_limits<float>::infinity();
    VisibilityType visibility = VisibilityType::Visible;

protected:
    Impl() = default;
    Impl(const Impl&) = default;
    Impl& operator=(const Impl&) = delete;

    // Stores what render passes this layer is currently enabled for. This depends on the
    // evaluated StyleProperties object and is updated accordingly.
    RenderPass passes = RenderPass::None;
};

} // namespace mbgl

#endif