/* This is a stateful component. In ReasonReact, we call them reducer components */
/* A list of state transitions, to be used in self.reduce and reducer */
let tickTimeInMs = 10;

let msInSec = 1000;

let seconds = 10;

let rest = msInSec / tickTimeInMs;

let totalCount = seconds * rest * tickTimeInMs;

type action =
  | Tick;

/* The component's state type. It can be anything, including, commonly, being a record type */
type state = {
  total: int,
  timerId: ref (option Js.Global.intervalId)
};

let component = ReasonReact.reducerComponent "Counter";

let make _children => {
  ...component,
  initialState: fun () => {total: totalCount, timerId: ref None},
  reducer: fun action state =>
    switch action {
    | Tick =>
      if (state.total <= 0) {
        switch !state.timerId {
        | Some id => Js.Global.clearInterval id
        | _ => ()
        };
        ReasonReact.NoUpdate
      } else {
        ReasonReact.Update {...state, total: state.total - tickTimeInMs}
      }
    },
  didMount: fun self => {
    self.state.timerId := Some (Js.Global.setInterval (self.reduce (fun _ => Tick)) tickTimeInMs);
    ReasonReact.NoUpdate
  },
  render: fun {state: {total}} => {
    let seconds = total / 1000;
    let percentage = float_of_int (total / 100) /. 100.0;
    let timesMessage = seconds == 1 ? "second" : "seconds";
    let greeting = {j|$seconds $timesMessage|j};
    /* if (seconds <= 0) {
         <div> (ReasonReact.stringToElement "Uploaded") </div>
       } else { */
    <div className="upload">
      <div
        className="upload__progress"
        style=(ReactDOMRe.Style.make transform::("scaleX(" ^ string_of_float percentage ^ ")") ())
      />
      <h1 className="upload__text"> (ReasonReact.stringToElement greeting) </h1>
    </div>
    /* } */
  }
};
