console.log("JS is loaded!")

const imageInput = document.getElementById('image-input')
const uploadBtn = document.getElementById('upload-btn')
const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const downloadBtn = document.getElementById('download-btn')
const mirrorBtn = document.getElementById('mirror-btn')
let PPMString = ""

uploadBtn.addEventListener('click', () => {
    const file = imageInput.files[0];
    console.log(file)
    displayImage(file)
});

downloadBtn.addEventListener('click', () => {
    const blob = new Blob([PPMString], { type: "image/x-portable-pixmap" });
    const url = URL.createObjectURL(blob)
    const a = document.createElement('a')
    a.href = url
    a.download = 'download.ppm'
    a.click()
    window.URL.revokeObjectURL(url);
});

mirrorBtn.addEventListener('click', () => {
    stringPtr = stringToNewUTF8(PPMString)
    try {
        resultPtr = Module._mirrorPPM(stringPtr)
        PPMString = UTF8ToString(resultPtr)
        Module._free(resultPtr)
    } catch (error) {
        console.warn("something went wrong! ", error)
    }
    console.log("mirroring complete!")
});

const displayImage = (file) => {
    const image = new Image();
    const url = URL.createObjectURL(file)
    image.src = url;

    image.addEventListener("load", () => {
        canvas.width = image.width
        canvas.height = image.height
        ctx.drawImage(image, 0, 0, canvas.width, canvas.height);
        const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height).data;
        PPMString = getPPMString(imageData)
    });
}

const getPPMString = (pxData) => {
    let result = ""
    result += "P3\n"
    result += `${canvas.width} ${canvas.height}\n`
    result += "255\n"
    for (let i = 0; i < pxData.length; i++) {
        if ((i + 1) % 4 == 0) continue
        result += pxData[i] + ' '
    }
    return result
}
