#ifndef MBGL_CUSTOM_LAYER
#define MBGL_CUSTOM_LAYER

#include <mbgl/style/style_layer.hpp>

namespace mbgl {

class TransformState;

class CustomLayer : public StyleLayer {
public:
    CustomLayer(const std::string& id,
                CustomLayerInitializeFunction,
                CustomLayerRenderFunction,
                CustomLayerDeinitializeFunction,
                void* context);

    CustomLayer(const CustomLayer&);
    ~CustomLayer();

    void initialize();
    void render(const TransformState&) const;

private:
    std::unique_ptr<StyleLayer> clone() const final;

    void parseLayout(const JSVal&) final {}
    void parsePaints(const JSVal&) final {}

    void cascade(const StyleCascadeParameters&) final {}
    bool recalculate(const StyleCalculationParameters&) final;

    std::unique_ptr<Bucket> createBucket(StyleBucketParameters&) const final;

    CustomLayerInitializeFunction initializeFn;
    CustomLayerRenderFunction renderFn;
    CustomLayerDeinitializeFunction deinitializeFn;
    void* context;
};

} // namespace mbgl

#endif
