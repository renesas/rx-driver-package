## Description
- Author : `(Write the person responsible for submitting the MR)`
- Contents : `(Write what updates are included in the MR)`
- Review-by date : `(Set review deadline [include time of feedback, fix it and code is merged] [Japan team needs 2 weeks to review])`

## Checklist:
- [ ] Have you conducted an internal review?
- [ ] Have you performed the simple tests to ensure that it was working properly?
- [ ] Have you confirmed the need to update the AD/UD specs?
- [ ] Have you confirmed the need to implement configAssert() to prevent RAM destruction?
   - Implementation details : `Set "No need" or "Add configAssert()"`
- [ ] Have you checked the return value of the API function to make sure it branches to the correct process?
   - Implementation details : `Set "No need" or "Add branches"`
- [ ] Have you confirmed the need to implement configAssert() and set local variables to NULL or suitable value as the initial value to prevent forgetting to check for errors?
   - Implementation details : `Set "No need" or "Add configAssert(), initialize local val"`
- [ ] Have you confirmed the need to implement configAssert() to check enough memory available at the copy destination?
   - Implementation details : `Set "No need" or "Add configAssert()"`
- [ ] When you make the same change to multiple projects, did you check that the change was made correctly?
   - Implementation details : `Write "No need" or "Name of Applicable projects"`
- [ ] Have you checked that you are using standard functions and each API correctly?
- [ ] Have you verified that your changes work correctly in all flows?

## Internal review
- Internal review minutes : `Attach peer review minutes or [Internal Review]MR!`

## Static analysis
- Static analysis : `(Set the whether static analysis was performed as Yes or No [static analysis must be performed for every MR])`
   - Codan check : `(Attach the static analysis results report)`
   - C++ Test : `(Attach the static analysis results report)`

## Simple tests
- Simple tests : `(Set the whether simple tests was performed as Yes or No)`
   - Test resutls : `(Attach the static simple test results report)`

## AD/UD specs update
- AD/UD specs update : `(Set whether AD/UD specs has been updated as Yes or No)`
   - Update details : `(Write details of the AD/UD specs updates)` 

## Related issue
- `(Write the issue number included in the MR [Set one issue for one MR])`