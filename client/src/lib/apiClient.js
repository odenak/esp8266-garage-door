const URLS = {
    doors: '/api/doors',
    door: '/api/door'
}

export default class ApiClient {
    static getAllStatus() {
        return fetch(URLS.doors)
            .then(
                res => {
                    switch (res.status) {
                        case 200:
                            return res.json()
                        default:
                            throw new Error("There was an unknown Error")
                    }
                })

    }

    static pushButtonForDoor(buttonId) {
        const url = `${URLS.door}/${buttonId}`

        return fetch(url, { method: 'POST' })
            .then(
                res => {
                    switch (res.status) {
                        case 200:
                            return res.json()
                        default:
                            throw new Error("There was an unknown Error")
                    }
                })
    }
}
