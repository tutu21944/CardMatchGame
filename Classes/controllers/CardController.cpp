#include "CardController.h"
#include "views/CardView.h"

void CardController::updateView()
{
    if (_view && _model)
    {
        _view->updateView();
    }
}