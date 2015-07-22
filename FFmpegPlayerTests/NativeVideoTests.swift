//  Created by Roman Petrov on 21/07/15.
//  Copyright (c) 2015 Kentor. All rights reserved.

import UIKit
import XCTest

class NativeVideoTests: XCTestCase {
    func testNativeFunctions_ok() {
        let videoPath:String = NSBundle.mainBundle().pathForResource("cat", ofType: "mp4")!
        
        XCTAssertTrue(native_video_init(videoPath), "native_video_init must return true")
        
        var videoRes:[Int32] = [0, 0]
        XCTAssertTrue(native_get_video_res(&videoRes), "native_get_video_res must return true")
        XCTAssertEqual(640, videoRes[0])
        XCTAssertEqual(352, videoRes[1])
        
        XCTAssertTrue(native_setup_scaler(videoRes[0], videoRes[1]), "native_setup_scaler must return true")

        XCTAssertTrue(native_set_seek_time(0.0), "native_set_seek_time must return true")

        XCTAssertTrue(native_step_frame(), "native_step_frame must return true")
        let pict = native_get_current_frame()
        XCTAssertEqual(1920, pict.memory.linesize.0)
    }
}
