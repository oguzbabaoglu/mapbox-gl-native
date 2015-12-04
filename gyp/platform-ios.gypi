{
  'targets': [
    { 'target_name': 'platform-ios',
      'product_name': 'mbgl-platform-ios',
      'type': 'static_library',
      'standalone_static_library': 1,
      'hard_dependency': 1,
      'dependencies': [
        'version',
      ],

      'sources': [
        '../platform/default/async_task.cpp',
        '../platform/default/run_loop.cpp',
        '../platform/default/timer.cpp',
        '../platform/darwin/log_nslog.mm',
        '../platform/darwin/string_nsstring.mm',
        '../platform/darwin/application_root.mm',
        '../platform/darwin/asset_root.mm',
        '../platform/darwin/image.mm',
        '../platform/darwin/nsthread.mm',
        '../platform/darwin/reachability.m',
        '../platform/darwin/NSException+MGLAdditions.h',
        '../platform/darwin/NSString+MGLAdditions.h',
        '../platform/darwin/NSString+MGLAdditions.m',
        '../platform/darwin/MGLTypes.m',
        '../platform/darwin/MGLStyle.mm',
        '../platform/darwin/MGLGeometry_Private.h',
        '../platform/darwin/MGLGeometry.mm',
        '../platform/darwin/MGLShape.m',
        '../platform/darwin/MGLMultiPoint_Private.h',
        '../platform/darwin/MGLMultiPoint.mm',
        '../platform/darwin/MGLPointAnnotation.m',
        '../platform/darwin/MGLPolyline.mm',
        '../platform/darwin/MGLPolygon.mm',
        '../platform/darwin/MGLMapCamera.mm',
        '../platform/ios/MGLMapboxEvents.h',
        '../platform/ios/MGLMapboxEvents.m',
        '../platform/ios/MGLMapView.mm',
        '../platform/ios/MGLAccountManager_Private.h',
        '../platform/ios/MGLAccountManager.m',
        '../platform/ios/MGLUserLocation_Private.h',
        '../platform/ios/MGLUserLocation.m',
        '../platform/ios/MGLUserLocationAnnotationView.h',
        '../platform/ios/MGLUserLocationAnnotationView.m',
        '../platform/ios/MGLAnnotationImage.m',
        '../platform/ios/NSBundle+MGLAdditions.h',
        '../platform/ios/NSBundle+MGLAdditions.m',
        '../platform/ios/NSProcessInfo+MGLAdditions.h',
        '../platform/ios/NSProcessInfo+MGLAdditions.m',
        '../platform/ios/vendor/SMCalloutView/SMCalloutView.h',
        '../platform/ios/vendor/SMCalloutView/SMCalloutView.m',
        '../platform/ios/vendor/Fabric/FABAttributes.h',
        '../platform/ios/vendor/Fabric/FABKitProtocol.h',
        '../platform/ios/vendor/Fabric/Fabric.h',
        '../platform/ios/vendor/Fabric/Fabric+FABKits.h',
      ],

      'variables': {
        'cflags_cc': [
          '<@(libuv_cflags)',
          '<@(boost_cflags)',
          '<@(variant_cflags)',
        ],
        'libraries': [
          '<@(libuv_static_libs)',
          '$(SDKROOT)/System/Library/Frameworks/CoreGraphics.framework',
          '$(SDKROOT)/System/Library/Frameworks/CoreLocation.framework',
          '$(SDKROOT)/System/Library/Frameworks/GLKit.framework',
          '$(SDKROOT)/System/Library/Frameworks/ImageIO.framework',
          '$(SDKROOT)/System/Library/Frameworks/MobileCoreServices.framework',
          '$(SDKROOT)/System/Library/Frameworks/OpenGLES.framework',
          '$(SDKROOT)/System/Library/Frameworks/QuartzCore.framework',
          '$(SDKROOT)/System/Library/Frameworks/Security.framework',
          '$(SDKROOT)/System/Library/Frameworks/SystemConfiguration.framework',
          '$(SDKROOT)/System/Library/Frameworks/UIKit.framework',
        ],
      },

      'include_dirs': [
        '../include/mbgl/ios',
        '../include/mbgl/darwin',
        '../include',
        '../src',
      ],

      'xcode_settings': {
        'OTHER_CPLUSPLUSFLAGS': [ '<@(cflags_cc)' ],
        'CLANG_ENABLE_OBJC_ARC': 'YES',
        'CLANG_ENABLE_MODULES': 'YES',
      },

      'link_settings': {
        'libraries': [ '<@(libraries)' ],
      },

      'direct_dependent_settings': {
        'include_dirs': [
          '../include/mbgl/ios',
          '../include/mbgl/darwin',
          '../include',
        ],
        'mac_bundle_resources': [
          '<!@(find ../platform/ios/resources -type f \! -name "README" \! -name \'.*\')',
        ],
      },
    },
  ],
}
