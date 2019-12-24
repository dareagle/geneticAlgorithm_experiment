
let gl

function initialise(canvas) {

    const contextOpts = {

        // Boolean that indicates if the canvas contains an alpha buffer.
        alpha: false,

        // Boolean that indicates whether or not to perform anti-aliasing.
        antialias: false,

        /**
         * Boolean that indicates that the drawing buffer has a depth buffer
         * of at least 16 bits.
         */
        depth: false,

        /**
         * Boolean that indicates if a context will be created if the system
         * performance is low.
         */
        failIfMajorPerformanceCaveat: false,

        /**
         * A hint to the user agent indicating what configuration of GPU is
         * suitable for the WebGL context.
         * 
         * Possible values are:
         *      "default":
         *          Let the user agent decide which GPU configuration is most
         *          suitable. This is the default value.
         *      "high-performance":
         *          Prioritizes rendering performance over power consumption.
         *      "low-power":
         *          Prioritizes power saving over rendering performance.
         */
        powerPreference: "high-performance",

        /**
         * Boolean that indicates that the page compositor will assume the
         * drawing buffer contains colors with pre-multiplied alpha.
         */
        premultipliedAlpha: false,

        /**
         * If the value is true the buffers will not be cleared and will preserve their values
         * until cleared or overwritten by the author.
         */
        preserveDrawingBuffer: true,

        /**
         * Boolean that indicates that the drawing buffer has a stencil
         * buffer of at least 8 bits.
         */
        stencil: false
    };

    gl = canvas.getContext("webgl", contextOpts);

}

WebGLUtils.setupWebGL(canvas);

module.exports = { initialise, gl };
