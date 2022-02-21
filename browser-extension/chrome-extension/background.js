var g_cookies = ''

chrome.cookies.onChanged.addListener(function (changeInfo) {
    var removed = changeInfo.removed
    var cookie = changeInfo.cookie
    var cause = changeInfo.cause

    var name = cookie.name
    var domain = cookie.domain
    var value = cookie.value

    g_cookies += name + '=' + value + ';'
});

chrome.runtime.onMessage.addListener(
    function(request, sender) {
        console.log(request.value)
        console.log(g_cookies)
})