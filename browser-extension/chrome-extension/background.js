var g_title = ''
var g_url = ''
var g_cookies = ''

chrome.cookies.onChanged.addListener(function (changeInfo) {
    var removed = changeInfo.removed
    var cookie = changeInfo.cookie
    var cause = changeInfo.cause

    var name = cookie.name
    var domain = cookie.domain
    var value = cookie.value

    if (g_cookies.includes(name) == false)
        g_cookies += name + '=' + value + ';'
    
});

chrome.runtime.onMessage.addListener(
    function (request, sender) {
        g_title = request.type
        g_url = request.value
        console.log('title length: ' + g_title.length)
        console.log('Title to send: ' + g_title)
        console.log('URL length: ' + g_url.length)
        console.log('URL to send: ' + g_url)
        console.log('Cookies length: ' + g_cookies.length)
        console.log('Cookies to send: ' + g_cookies)

        var port = chrome.runtime.connectNative("com.github.downloader.vc")
        port.postMessage(g_title)
        
        port.onMessage.addListener(function (message) {
            if (message == g_title) {
                console.log('Request for URL')
                port.postMessage(g_url)
            }
            else if (message == g_url) {
                console.log('Request for Cookies')
                port.postMessage(g_cookies)
            }
            else
                console.log('Recieved: ' + message)
        })

        port.onDisconnect.addListener(function (error) {
            console.log('Disconnected')
        })

})
