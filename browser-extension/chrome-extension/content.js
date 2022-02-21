let vcmPattern = /^(https):\/\/(vcm).*/gm
let docUrl = document.URL

if (docUrl.search(vcmPattern) == 0) {
    var userAns = window.confirm("آیا تمایل به دانلود این کلاس دارید؟")
    if (userAns == true) {
        chrome.runtime.sendMessage({
            type: 'url',
            value: document.URL
        });
    }
}
