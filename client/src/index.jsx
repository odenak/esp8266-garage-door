import React from 'react'
import ApiClient from './libs/apiClient'

export default class Main extends React.Component {

    constructor() {
        super()
        this.state = {
            doors: []
        }
    }


    componentDidMount() {
        this.getAllStatus()
    }

    getAllStatus() {
        ApiClient.getAllStatus()
            .then(
                doors => {
                    this.setState({ doors })
                })
            .catch(
                err => {
                    console.log(err)
                })

    }

    handleButton(buttonId) {
        ApiClient.pushButtonForDoor(buttonId)
            .then(
                doors => {
                    this.setState({ doors })
                })
            .catch(
                err => {
                    console.log(err)
                })

    }

    render() {
        return (
            <div id="app">
                {
                    this.state.doors.map((door, id) => (
                        <div key={id}>
                            <h2>{door.name}</h2>
                            <div>Status: {door.open ? 'Opened' : 'Closed'}</div>
                            <button onClick={() => this.handleButton(id)}>Push Button</button>
                            <hr/>
                        </div>
                    ))
                }
            </div>
        );
    }
}
