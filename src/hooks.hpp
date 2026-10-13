#pragma once

namespace Hooks {
    void Install();
    // call from game loop (LateUpdate / CADisplayLink)
    void OnFrame(float screen_w, float screen_h);
}
